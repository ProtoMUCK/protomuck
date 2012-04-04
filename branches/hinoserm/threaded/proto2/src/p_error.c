/* Primitives Package */

#include "copyright.h"
#include "config.h"

#include "db.h"
#include "tune.h"
#include "inst.h"
#include "externs.h"
#include "match.h"
#include "interface.h"
#include "params.h"
#include "strings.h"
#include "interp.h"

union error_mask {
    struct {
        unsigned int div_zero:1; /* Divide by zero */
        unsigned int nan:1;     /* Result would not be a number */
        unsigned int imaginary:1; /* Result would be imaginary */
        unsigned int f_bounds:1; /* Float boundary error */
        unsigned int i_bounds:1; /* Integer boundary error */
    } error_flags;
    int is_flags;
};

#define ERROR_NAME_MAX_LEN 15
#define ERROR_STRING_MAX_LEN 80
#define ERROR_NUM 5

union error_mask err_bits[ERROR_NUM];

struct err_type {
    int error_bit;
    char error_name[ERROR_NAME_MAX_LEN];
    char error_string[ERROR_STRING_MAX_LEN];
};

static struct err_type err_defs[] = {
    {0, "DIV_ZERO", "Division by zero attempted."},
    {1, "NAN", "Result was not a number."},
    {2, "IMAGINARY", "Result was imaginary."},
    {3, "FBOUNDS", "Floating-point inputs were infinite or out of range."},
    {4, "IBOUNDS", "Calculation resulted in an integer overflow."},
    {5, "", ""}
};

static int err_init = 0;

void
init_err_val(void)
{
    int i;

    for (i = 0; i < ERROR_NUM; i++)
        err_bits[i].is_flags = 0;
    err_bits[0].error_flags.div_zero = 1;
    err_bits[1].error_flags.nan = 1;
    err_bits[2].error_flags.imaginary = 1;
    err_bits[3].error_flags.f_bounds = 1;
    err_bits[4].error_flags.i_bounds = 1;
    err_init = 1;
}

void
prim_clear(PRIM_PROTOTYPE)
{
    fr->error.is_flags = 0;
}

void
prim_error_num(PRIM_PROTOTYPE)
{
	int result;

    CHECKOFLOW(1);
    result = ERROR_NUM;
    PushInt(result);
}

void
prim_clear_error(PRIM_PROTOTYPE)
{
    int loop, result;
	char buf[BUFFER_LEN];

    if (oper[0].type != PROG_STRING && oper[0].type != PROG_INTEGER)
        abort_interp("Invalid argument type. (1)");
    if (!err_init)
        init_err_val();
    if (oper[0].type == PROG_INTEGER) {
        if ((oper[0].data.number < 0) || (oper[0].data.number >= ERROR_NUM)) {
            result = 0;
        } else {
            fr->error.is_flags =
                fr->error.is_flags & (~err_bits[oper[0].data.number].is_flags);
            result = 1;
        }
    } else {
        if (!oper[0].data.string) {
            result = 0;
        } else {
            loop = 0;
            result = strlen(oper[0].data.string->data);
            for (loop = 0; loop < result; loop++)
                buf[loop] = toupper(oper[0].data.string->data[loop]);
            result = 0;
            loop = 0;
            while (loop < ERROR_NUM) {
                if (!strcmp(buf, err_defs[loop].error_name)) {
                    result = 1;
                    fr->error.is_flags =
                        fr->error.is_flags & (~err_bits[loop].is_flags);
					break;
                } else {
                    loop++;
                }
            }
        }
    }
    PushInt(result);
}

void
prim_set_error(PRIM_PROTOTYPE)
{
    int loop, result;
	char buf[BUFFER_LEN];

    if (oper[0].type != PROG_STRING && oper[0].type != PROG_INTEGER)
        abort_interp("Invalid argument type. (1)");
    if (!err_init)
        init_err_val();
    if (oper[0].type == PROG_INTEGER) {
        if ((oper[0].data.number < 0) || (oper[0].data.number >= ERROR_NUM)) {
            result = 0;
        } else {
            fr->error.is_flags =
                fr->error.is_flags | err_bits[oper[0].data.number].is_flags;
            result = 1;
        }
    } else {
        if (!oper[0].data.string) {
            result = 0;
        } else {
            loop = 0;
            result = strlen(oper[0].data.string->data);
            for (loop = 0; loop < result; loop++)
                buf[loop] = toupper(oper[0].data.string->data[loop]);
            result = 0;
            loop = 0;
            while (loop < ERROR_NUM) {
                if (!strcmp(buf, err_defs[loop].error_name)) {
                    result = 1;
                    fr->error.is_flags =
                        fr->error.is_flags | err_bits[loop].is_flags;
					break;
                } else {
                    loop++;
                }
            }
        }
    }
    PushInt(result);
}

void
prim_is_set(PRIM_PROTOTYPE)
{
    int loop, result;
	char buf[BUFFER_LEN];

    if (oper[0].type != PROG_STRING && oper[0].type != PROG_INTEGER)
        abort_interp("Invalid argument type. (1)");
    if (!err_init)
        init_err_val();
    if (oper[0].type == PROG_INTEGER) {
        if ((oper[0].data.number < 0) || (oper[0].data.number >= ERROR_NUM)) {
            result = 0;
        } else {
            result =
                ((fr->error.is_flags & err_bits[oper[0].data.number].is_flags) !=
                 0);
        }
    } else {
        if (!oper[0].data.string) {
            result = 0;
        } else {
            loop = 0;
            result = strlen(oper[0].data.string->data);
            for (loop = 0; loop < result; loop++)
                buf[loop] = toupper(oper[0].data.string->data[loop]);
            result = 0;
            loop = 0;
            while (loop < ERROR_NUM) {
                if (!strcmp(buf, err_defs[loop].error_name)) {
                    result =
                        ((fr->error.is_flags & err_bits[loop].is_flags) != 0);
					break;
                } else {
                    loop++;
                }
            }
        }
    }
    PushInt(result);
}

void
prim_error_str(PRIM_PROTOTYPE)
{
    int loop, result;
	char buf[BUFFER_LEN];

    if (oper[0].type != PROG_STRING && oper[0].type != PROG_INTEGER)
        abort_interp("Invalid argument type. (1)");
    if (!err_init)
        init_err_val();
    if (oper[0].type == PROG_INTEGER) {
        if ((oper[0].data.number < 0) || (oper[0].data.number >= ERROR_NUM)) {
            result = -1;
        } else {
            result = oper[0].data.number;
        }
    } else {
        if (!oper[0].data.string) {
            result = -1;
        } else {
            loop = 0;
            result = strlen(oper[0].data.string->data);
            for (loop = 0; loop < result; loop++)
                buf[loop] = toupper(oper[0].data.string->data[loop]);
            result = -1;
            loop = 0;
            while (loop < ERROR_NUM) {
                if (!strcmp(buf, err_defs[loop].error_name)) {
                    result = loop;
                    break;
                } else {
                    loop++;
                }
            }
        }
    }
    if (result == -1) {
        PushNullStr;
    } else {
        PushString(err_defs[result].error_string);
    }
}

void
prim_error_name(PRIM_PROTOTYPE)
{
	int result;

    if (oper[0].type != PROG_INTEGER)
        abort_interp("Invalid argument type. (1)");
    if (!err_init)
        init_err_val();
    if ((oper[0].data.number < 0) || (oper[0].data.number >= ERROR_NUM)) {
        result = -1;
    } else {
        result = oper[0].data.number;
    }
    if (result == -1) {
        PushNullStr;
    } else {
        PushString(err_defs[result].error_name);
    }
}

void
prim_error_bit(PRIM_PROTOTYPE)
{
    int loop, result;
	char buf[BUFFER_LEN];

    if (oper[0].type != PROG_STRING)
        abort_interp("Invalid argument type. (1)");
    if (!err_init)
        init_err_val();
    if (!oper[0].data.string) {
        result = -1;
    } else {
        loop = 0;
        result = strlen(oper[0].data.string->data);
        for (loop = 0; loop < result; loop++)
            buf[loop] = toupper(oper[0].data.string->data[loop]);
        result = -1;
        loop = 0;
        while (loop < ERROR_NUM) {
            if (!strcmp(buf, err_defs[loop].error_name)) {
                result = loop;
				break;
            } else {
                loop++;
            }
        }
    }
    PushInt(result);
}

void
prim_is_error(PRIM_PROTOTYPE)
{
	int result;

    CHECKOFLOW(1);
    result = ((fr->error.is_flags) != 0);
    PushInt(result);
}