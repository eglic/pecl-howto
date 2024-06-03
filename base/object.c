#include "base.h"
#define OBJECT_POOL_UNIT_MIN  32
#define OBJECT_POOL_UNIT_MAX  2048
#define OBJECT_POOL_COUNT_MIN 4
#define OBJECT_POOL_COUNT_MAX 128

/**
 * @brief 创建一个对象池
 * @param unitSize 对象大小,最小分配 32字节,最大不建议超过 512 字节 , 按16字节对齐
 * @param unitCount 预计对象数量，只是初始设置
 * @return
 */
z_node *object_pool_create( uint16_t unitSize, uint16_t unitCount ) {
    uint16_t UL = unitSize;
    if ( UL < OBJECT_POOL_UNIT_MIN )
        UL = OBJECT_POOL_UNIT_MIN;
    if ( UL > OBJECT_POOL_UNIT_MAX ) // 超限，不建议用我
        return NULL;
    if ( ( UL & 0x0f ) != 0 ) { // 没对齐
        if ( UL < 0x10 ) {
            UL = 0x10;
        } else {
            UL &= 0xfffffff0;
            UL += 0x10;
        }
    }

    uint16_t UC = unitCount;
    if ( UC < OBJECT_POOL_COUNT_MIN )
        UC = OBJECT_POOL_COUNT_MIN;
    if ( UC > OBJECT_POOL_COUNT_MAX )
        UC = OBJECT_POOL_COUNT_MAX;
    uint32_t RS     = UC;      // bin 区先分配 N 字节标志位
    uint32_t RC     = UL * UC; // bin 去再分配数据区域
    size_t   TS     = sizeof( z_node ) + RS + RC;
    z_node  *bucket = malloc( TS );
    memset( bucket, 0, TS );
    bucket->cap      = RC;
    bucket->unit.len = UL;
    bucket->unit.cnt = UC;
    return bucket;
}

/**
 * @brief 销毁对象池
 * @param pool
 */
void object_pool_destroy( z_node *pool ) {
    if ( pool == NULL )
        return;
    z_node *backup_n = pool->next, *backup_p = pool->prev, *p = NULL;
    if ( backup_n == pool )
        backup_n = NULL;
    if ( backup_p == pool )
        backup_p = NULL;

    // 先干掉自己
    free( pool );
    if ( backup_n != NULL )
        backup_n->prev = NULL;
    if ( backup_p != NULL )
        backup_p->next = NULL;

    // 往后销毁
    while ( backup_n != NULL ) {
        p = backup_n->next;
        if ( p == backup_p ) // 防止成环
            backup_p = NULL;
        free( backup_n );
        backup_n = p;
    }

    // 往前销毁
    while ( backup_p != NULL ) {
        p = backup_p->prev;
        free( backup_p );
        backup_p = p;
    }
}

/**
 * @brief 在本体寻找
 * @param poo
 * @param UL
 * @return
 */
static void *object_pool_borrow_self( z_node *pool, uint16_t UL ) {
    // [1] 直接找
    unsigned char *seat = pool->bin;                  // 索引就在 0 偏移位置
    unsigned char *data = pool->bin + pool->unit.cnt; // 数据在 n 偏移位置
    for ( uint16_t i = 0; i < pool->unit.cnt; i++, data += pool->unit.len ) {
        if ( seat[i] == 0 ) {
            seat[i] = 1;
            return data;
        }
    }

    // [2] 没找到，本地满了
    // 能到这里来，UL 肯定小于我自己 , 先看左侧

    // [2.1] 左侧空，新分配一个就行了
    if ( pool->left == NULL ) {
        z_node *n  = object_pool_create( UL, pool->unit.cnt ); // 搞一个跟我一样大的
        pool->left = n;
        n->right   = pool;
        n->left    = NULL;
        return object_pool_borrow_self( n, UL );
    }

    // [2.2] 左侧非空，且空间够，就地解决
    if ( pool->left->cap >= UL ) {
        return object_pool_borrow_self( pool->left, UL );
    }

    // [2.3] 左侧有，且小，需要在中间插入一个
    z_node *n  = object_pool_create( UL, pool->unit.cnt ); // 搞一个跟我一样大的
    z_node *l  = pool->left;
    pool->left = n;
    n->left    = l;
    l->right   = n;
    n->right   = pool;
    return object_pool_borrow_self( n, UL );
}

/**
 * @brief 尝试向左侧寻找
 * @param pool
 * @param UL
 * @return
 */
static void *object_pool_borrow_left( z_node *pool, uint16_t UL ) {
    if ( pool->left == NULL ) {
        z_node *n  = object_pool_create( UL, pool->unit.cnt ); // 搞一个跟我一样大的
        pool->left = n;
        n->right   = pool;
        n->left    = NULL;
        return object_pool_borrow_self( n, UL );
    }
    // 如果 左桶的空间小于需求空间，回去
    if ( pool->left->unit.len < UL ) { // 左侧空间小于本地，回去
        return object_pool_borrow_self( pool, UL );
    } else if ( pool->left->unit.len == UL ) { // 左侧空间刚好，就地解决
        return object_pool_borrow_self( pool->left, UL );
    } else if ( pool->left->left != NULL ) { // 左侧空间偏大，但是没有更左，尝试递归回来分配
        return object_pool_borrow_left( pool->left, UL );
    } else { // 左侧空间偏大，且有更左，先本地解决再说
        return object_pool_borrow_self( pool->left, UL );
    }
}

/**
 * @brief 尝试向右侧寻找
 * @param pool
 * @param UL
 * @return
 */
static void *object_pool_borrow_right( z_node *pool, uint16_t UL ) {
    if ( pool->right == NULL ) {
        z_node *n   = object_pool_create( UL, pool->unit.cnt ); // 搞一个跟我一样大的
        pool->right = n;
        n->left     = pool;
        n->right    = NULL;
        return object_pool_borrow_self( n, UL );
    }
    if ( pool->right->unit.len < UL ) { // 继续向右
        return object_pool_borrow_right( pool->right, UL );
    } else { // 越往后越大，就近分配
        return object_pool_borrow_self( pool->right, UL );
    }
}

/**
 * @brief 去对象池中借一个对象
 * @param pool
 * @param size
 * @return
 */
void *object_pool_borrow( z_node *pool, uint16_t unitSize ) {
    if ( pool == NULL )
        return NULL;
    uint16_t UL = unitSize;
    if ( UL < OBJECT_POOL_UNIT_MIN )
        UL = OBJECT_POOL_UNIT_MIN;
    if ( UL > OBJECT_POOL_UNIT_MAX ) // 超限，不建议用我
        return NULL;
    if ( UL == pool->unit.len ) { // 在本体寻找
        return object_pool_borrow_self( pool, UL );
    } else if ( UL < pool->unit.len ) {
        return object_pool_borrow_left( pool, UL );
    } else {
        return object_pool_borrow_right( pool, UL );
    }
}

/**
 * @brief 向对象池归还
 * @param pool
 * @param ptr
 */
bool object_pool_return( z_node *pool, void *ptr ) {
    if ( ptr == NULL || pool == NULL ) {
        // 搞不定，骗客户搞定了
        return true;
    }
    unsigned char *SEAT    = pool->bin;                  // 索引就在 0 偏移位置
    void          *D_START = pool->bin + pool->unit.cnt; // 指针开始位置
    void          *D_END   = pool->bin + pool->unit.cnt + pool->unit.cnt * pool->unit.len; // 指针结束位置

    // 就在这里
    if ( ptr >= D_START && ptr <= D_END ) {
        uint32_t off = ptr - D_START;
        uint32_t pos = off / pool->unit.len;
        if ( pos == pool->unit.len ) // 上面判断的 <= 可能会导致正好卡在边界上
            pos--;

        if ( SEAT[pos] == 0 ) {
            // 意外了，暂时不管
            // 看成是多次归还
        }
        SEAT[pos] = 0;
        return true;
    }

    // 不在，分头寻找
    z_node *p = pool->left;
    while ( p != NULL ) {
        if ( object_pool_return( p, ptr ) )
            return true;
        p = p->left;
    }

    // 左边也没有，往右找
    p = pool->right;
    while ( p != NULL ) {
        if ( object_pool_return( p, ptr ) )
            return true;
        p = p->right;
    }

    // 都没有，算了
    return false;
}
