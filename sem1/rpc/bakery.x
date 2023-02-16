
#define get_number 0
#define get_symbol 1
#define max_client 5

struct BAKERY
{
    int op;
    int num;
    int pid;
    int result;
};

program BAKERY_PROG
{
    version BAKERY_VER
    {
        struct BAKERY BAKERY_PROC(struct BAKERY) = 1;
    } = 1; /* Version number = 1 */
} = 0x20000001;