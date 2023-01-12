/*
 * filename: bakery.x
 * function: Define constants, non-standard data types and the calling process in remote calls
 */

struct bakery_t
{
    int op;
    int num;
    int result;
};

#define max_client 5
#define get_number 1
#define get_symbol 2

program BAKERY_PROG
{
    version BAKERY_VER
    {
        struct bakery_t BAKERY_PROC(struct bakery_t) = 1;
    } = 1; /* Version number = 1 */
} = 0x20000001; /* RPC program number */