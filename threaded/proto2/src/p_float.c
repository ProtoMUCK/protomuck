
/* Primitives Package */
#include "copyright.h"
#include "config.h"
#ifndef NAN
#include "nan.h"
#endif
#include "db.h"
#include "tune.h"
#include "inst.h"
#include "externs.h"
#include "match.h"
#include "interface.h"
#include "params.h"
#include "strings.h"
#include "interp.h"
#define no_good(x) ((x == INF) || (x == NINF) || (x == NAN))
#define ISINF(x) ((x == INF) || (x == NINF))
#define ISNAN(x) (x == NAN)
void
prim_ceil(PRIM_PROTOTYPE)
{
	double fresult;
    
    if (oper[0].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    if (!no_good(oper[0].data.fnumber)) {
        fresult = ceil((double) oper[0].data.fnumber);
    } else {
        fresult = oper[0].data.fnumber;
        fr->error.error_flags.f_bounds = 1;
    }
    
    PushFloat(fresult);
}
void
prim_floor(PRIM_PROTOTYPE)
{
	double fresult;
    
    if (oper[0].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    if (!no_good(oper[0].data.fnumber)) {
        fresult = floor((double) oper[0].data.fnumber);
    } else {
        fresult = oper[0].data.fnumber;
        fr->error.error_flags.f_bounds = 1;
    }
    
    PushFloat(fresult);
}
void
prim_sqrt(PRIM_PROTOTYPE)
{
	double fresult;
    
    if (oper[0].type == PROG_INTEGER) {
        oper[0].type = PROG_FLOAT;
        oper[0].data.fnumber = oper[0].data.number;
    }
    if (oper[0].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    if (!no_good(oper[0].data.fnumber)) {
        if (oper[0].data.fnumber < 0.0) {
            fresult = tp_alt_infinity_handler ? NAN : 0.0;
            fr->error.error_flags.imaginary = 1;
        } else {
            fresult = sqrt((double) oper[0].data.fnumber);
        }
    } else {
         if (ISNAN(oper[0].data.fnumber)) {
            fresult = tp_alt_infinity_handler ? NAN : 0.0;
            if (!tp_alt_infinity_handler) fr->error.error_flags.nan = 1;
         } else {
            fresult = tp_alt_infinity_handler ? INF : 0.0;
            if (!tp_alt_infinity_handler) fr->error.error_flags.f_bounds = 1;
         }
    }
    
    PushFloat(fresult);
}
void
prim_pi(PRIM_PROTOTYPE)
{
	double fresult;
    
    fresult = F_PI;
    CHECKOFLOW(1);
    PushFloat(fresult);
}
void
prim_epsilon(PRIM_PROTOTYPE)
{
	double fresult;
    fresult = DBL_EPSILON;
    CHECKOFLOW(1);
    PushFloat(fresult);
}
void
prim_round(PRIM_PROTOTYPE)
{
    double temp, tshift, tnum, fstore, fresult;
    
    if (oper[0].type != PROG_INTEGER)
        abort_interp("Non-integer argument. (2)");
    if (oper[1].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    if (oper[0].data.number < 0)
        abort_interp("Argument must be a positive integer. (2)");
    if (!no_good(oper[1].data.fnumber)) {
        temp = pow(10.0, oper[0].data.number);
        tshift = temp * (oper[1].data.fnumber);
        tnum = modf(tshift, &fstore);
        if (tnum >= 0.5) {
            fstore = fstore + 1.0;
        } else {
            if (tnum <= -0.5) {
                fstore = fstore - 1.0;
            }
        }
        fstore = fstore / temp;
        fresult = fstore;
    } else {
         if (ISNAN(oper[1].data.fnumber)) {
            fresult = tp_alt_infinity_handler ? NAN : 0.0;
            if (!tp_alt_infinity_handler) fr->error.error_flags.nan = 1;
         } else {
            fresult = tp_alt_infinity_handler ? INF : 0.0;
            if (!tp_alt_infinity_handler) fr->error.error_flags.f_bounds = 1;
         }
    }
    
    PushFloat(fresult);
}
void
prim_sin(PRIM_PROTOTYPE)
{
	double fresult;
    
    if (oper[0].type == PROG_INTEGER) {
        oper[0].type = PROG_FLOAT;
        oper[0].data.fnumber = oper[0].data.number;
    }
    if (oper[0].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    if (!no_good(oper[0].data.fnumber)) {
        fresult = sin(oper[0].data.fnumber);
    } else {
        /* TODO: This should be NaN -- Alynna: alright, its NaN */
            fresult = tp_alt_infinity_handler ? NAN : 0.0;
            if (!tp_alt_infinity_handler) fr->error.error_flags.nan = 1;
    }
    
    PushFloat(fresult);
}
void
prim_cos(PRIM_PROTOTYPE)
{
	double fresult;
    
    if (oper[0].type == PROG_INTEGER) {
        oper[0].type = PROG_FLOAT;
        oper[0].data.fnumber = oper[0].data.number;
    }
    if (oper[0].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    if (!no_good(oper[0].data.fnumber)) {
        fresult = cos(oper[0].data.fnumber);
    } else {
            fresult = tp_alt_infinity_handler ? NAN : 0.0;
            if (!tp_alt_infinity_handler) fr->error.error_flags.nan = 1;
    }
    
    PushFloat(fresult);
}
void
prim_tan(PRIM_PROTOTYPE)
{
	double fresult;
    
    if (oper[0].type == PROG_INTEGER) {
        oper[0].type = PROG_FLOAT;
        oper[0].data.fnumber = oper[0].data.number;
    }
    if (oper[0].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    if (!no_good(oper[0].data.fnumber)) {
        fresult = fmod((oper[0].data.fnumber - H_PI), F_PI);
        if (fabs(fresult) > DBL_EPSILON && fabs(fresult - F_PI) > DBL_EPSILON) {
            fresult = tan(oper[0].data.fnumber);
        } else {
            fresult = tp_alt_infinity_handler ? INF : 0.0;
            fr->error.error_flags.f_bounds = 1;
        }
    } else {
         if (ISNAN(oper[0].data.fnumber)) {
            fresult = tp_alt_infinity_handler ? NAN : 0.0;
            if (!tp_alt_infinity_handler) fr->error.error_flags.nan = 1;
         } else {
            fresult = tp_alt_infinity_handler ? INF : 0.0;
            if (!tp_alt_infinity_handler) fr->error.error_flags.f_bounds = 1;
         }
    }
    
    PushFloat(fresult);
}
void
prim_asin(PRIM_PROTOTYPE)
{
	double fresult;
    
    if (oper[0].type == PROG_INTEGER) {
        oper[0].type = PROG_FLOAT;
        oper[0].data.fnumber = oper[0].data.number;
    }
    if (oper[0].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    if ((oper[0].data.fnumber >= -1.0) && (oper[0].data.fnumber <= 1.0)) {
        fresult = asin(oper[0].data.fnumber);
    } else {
        fresult = tp_alt_infinity_handler ? NAN : 0.0;
        fr->error.error_flags.nan = 1;
    }
    
    PushFloat(fresult);
}
void
prim_acos(PRIM_PROTOTYPE)
{
	double fresult;
    
    if (oper[0].type == PROG_INTEGER) {
        oper[0].type = PROG_FLOAT;
        oper[0].data.fnumber = oper[0].data.number;
    }
    if (oper[0].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    if ((oper[0].data.fnumber >= -1.0) && (oper[0].data.fnumber <= 1.0)) {
        fresult = acos(oper[0].data.fnumber);
    } else {
        fresult = tp_alt_infinity_handler ? NAN : 0.0;
        fr->error.error_flags.nan = 1;
    }
    
    PushFloat(fresult);
}
void
prim_atan(PRIM_PROTOTYPE)
{
	double fresult;
    
    if (oper[0].type == PROG_INTEGER) {
        oper[0].type = PROG_FLOAT;
        oper[0].data.fnumber = oper[0].data.number;
    }
    if (oper[0].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    if (!no_good(oper[0].data.fnumber)) {
        fresult = atan(oper[0].data.fnumber);
    } else {
        fresult = H_PI;
    }
    
    PushFloat(fresult);
}
void
prim_atan2(PRIM_PROTOTYPE)
{
	double fresult;
    
    if (oper[1].type == PROG_INTEGER) {
        oper[1].type = PROG_FLOAT;
        oper[1].data.fnumber = oper[1].data.number;
    }
    if (oper[0].type == PROG_INTEGER) {
        oper[0].type = PROG_FLOAT;
        oper[0].data.fnumber = oper[1].data.number;
    }
    if (oper[1].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    if (oper[0].type != PROG_FLOAT)
        abort_interp("Non-float argument. (2)");
    fresult = atan2(oper[1].data.fnumber, oper[0].data.fnumber);
    
    PushFloat(fresult);
}
void
prim_dist3d(PRIM_PROTOTYPE)
{
    double dist;
    double x, y, z;
    
    if (oper[2].type == PROG_INTEGER) {
        oper[2].type = PROG_FLOAT;
        oper[2].data.fnumber = oper[2].data.number;
    }
    if (oper[1].type == PROG_INTEGER) {
        oper[1].type = PROG_FLOAT;
        oper[1].data.fnumber = oper[1].data.number;
    }
    if (oper[0].type == PROG_INTEGER) {
        oper[0].type = PROG_FLOAT;
        oper[0].data.fnumber = oper[0].data.number;
    }
    if (oper[2].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    if (oper[1].type != PROG_FLOAT)
        abort_interp("Non-float argument. (2)");
    if (oper[0].type != PROG_FLOAT)
        abort_interp("Non-float argument. (3)");
    x = oper[2].data.fnumber;
    y = oper[1].data.fnumber;
    z = oper[0].data.fnumber;
    dist = sqrt((x * x) + (y * y) + (z * z));
    
    PushFloat(dist);
}
void
prim_xyz_to_polar(PRIM_PROTOTYPE)
{
    double dist, theta, phi;
    double x, y, z;
    
    if (oper[2].type == PROG_INTEGER) {
        oper[2].type = PROG_FLOAT;
        oper[2].data.fnumber = oper[2].data.number;
    }
    if (oper[1].type == PROG_INTEGER) {
        oper[1].type = PROG_FLOAT;
        oper[1].data.fnumber = oper[2].data.number;
    }
    if (oper[0].type == PROG_INTEGER) {
        oper[0].type = PROG_FLOAT;
        oper[0].data.fnumber = oper[2].data.number;
    }
    if (oper[2].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    if (oper[1].type != PROG_FLOAT)
        abort_interp("Non-float argument. (2)");
    if (oper[0].type != PROG_FLOAT)
        abort_interp("Non-float argument. (3)");
    x = oper[2].data.fnumber;
    y = oper[1].data.fnumber;
    z = oper[0].data.fnumber;
    if (no_good(x) || no_good(y) || no_good(z)) {
        dist = 0.0;
        theta = 0.0;
        phi = 0.0;
        fr->error.error_flags.nan = 1;
    } else {
        dist = sqrt((x * x) + (y * y) + (z * z));
        if (dist > 0.0) {
            theta = atan2(y, x);
            phi = acos(z / dist);
        } else {
            theta = 0.0;
            phi = 0.0;
        }
    }
    
    PushFloat(dist);
    PushFloat(theta);
    PushFloat(phi);
}
void
prim_polar_to_xyz(PRIM_PROTOTYPE)
{
    double x, y, z;
    double dist, theta, phi;
    
    if (oper[2].type == PROG_INTEGER) {
        oper[2].type = PROG_FLOAT;
        oper[2].data.fnumber = oper[2].data.number;
    }
    if (oper[1].type == PROG_INTEGER) {
        oper[1].type = PROG_FLOAT;
        oper[1].data.fnumber = oper[2].data.number;
    }
    if (oper[0].type == PROG_INTEGER) {
        oper[0].type = PROG_FLOAT;
        oper[0].data.fnumber = oper[2].data.number;
    }
    if (oper[2].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    if (oper[1].type != PROG_FLOAT)
        abort_interp("Non-float argument. (2)");
    if (oper[0].type != PROG_FLOAT)
        abort_interp("Non-float argument. (3)");
    dist = oper[2].data.fnumber;
    theta = oper[1].data.fnumber;
    phi = oper[0].data.fnumber;
    if (no_good(dist) || no_good(theta) || no_good(phi)) {
        x = 0.0;
        y = 0.0;
        z = 0.0;
        fr->error.error_flags.nan = 1;
    } else {
        x = (dist * cos(theta) * sin(phi));
        y = (dist * sin(theta) * sin(phi));
        z = (dist * cos(phi));
    }
    
    PushFloat(x);
    PushFloat(y);
    PushFloat(z);
}
void
prim_exp(PRIM_PROTOTYPE)
{
	double fresult;
    
    if (oper[0].type == PROG_INTEGER) {
        oper[0].type = PROG_FLOAT;
        oper[0].data.fnumber = oper[0].data.number;
    }
    if (oper[0].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    if (!no_good(oper[0].data.fnumber)) {
        fresult = exp((double) oper[0].data.fnumber);
    } else if (ISINF(oper[0].data.fnumber)) {
        fresult = oper[0].data.fnumber;
        fr->error.error_flags.f_bounds = 1;
    } else {
        fresult = tp_alt_infinity_handler ? NAN : 0.0;
        fr->error.error_flags.f_bounds = 1;
    }
    
    PushFloat(fresult);
}
void
prim_log(PRIM_PROTOTYPE)
{
	double fresult;
    
    if (oper[0].type == PROG_INTEGER) {
        oper[0].type = PROG_FLOAT;
        oper[0].data.fnumber = oper[0].data.number;
    }
    if (oper[0].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    if (!no_good(oper[0].data.fnumber) && oper[0].data.fnumber > 0.0) {
        fresult = log(oper[0].data.fnumber);
    } else if (oper[0].data.fnumber > 0.0) {
        fresult = INF;
        fr->error.error_flags.f_bounds = 1;
    } else {
        fresult = tp_alt_infinity_handler ? NAN : 0.0;
        fr->error.error_flags.imaginary = 1;
    }
    
    PushFloat(fresult);
}
void
prim_log10(PRIM_PROTOTYPE)
{
	double fresult;
    
    if (oper[0].type == PROG_INTEGER) {
        oper[0].type = PROG_FLOAT;
        oper[0].data.fnumber = oper[0].data.number;
    }
    if (oper[0].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    if (!no_good(oper[0].data.fnumber) && oper[0].data.fnumber > 0.0) {
        fresult = (float) log10((double) oper[0].data.fnumber);
    } else if (oper[0].data.fnumber > 0.0) {
        fresult = INF;
        fr->error.error_flags.f_bounds = 1;
    } else {
        fresult = tp_alt_infinity_handler ? NAN : 0.0;
        fr->error.error_flags.imaginary = 1;
    }
    
    PushFloat(fresult);
}
void
prim_fabs(PRIM_PROTOTYPE)
{
	double fresult;
    
    if (oper[0].type == PROG_INTEGER) {
        oper[0].type = PROG_FLOAT;
        oper[0].data.fnumber = oper[0].data.number;
    }
    if (oper[0].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    if (!no_good(oper[0].data.fnumber)) {
        fresult = fabs(oper[0].data.fnumber);
    } else {
        fresult = oper[0].data.fnumber;
        fr->error.error_flags.f_bounds = 1;
    }
    
    PushFloat(fresult);
}
void
prim_float(PRIM_PROTOTYPE)
{
	double fresult;
    
    if (oper[0].type != PROG_INTEGER)
        abort_interp("Non-integer argument. (1)");
    fresult = oper[0].data.number;
    
    PushFloat(fresult);
}
void
prim_pow(PRIM_PROTOTYPE)
{
	double fresult;
    
    if (oper[0].type == PROG_INTEGER) {
        oper[0].type = PROG_FLOAT;
        oper[0].data.fnumber = oper[0].data.number;
    }
    if (oper[1].type == PROG_INTEGER) {
        oper[1].type = PROG_FLOAT;
        oper[1].data.fnumber = oper[1].data.number;
    }
    if (oper[1].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    if (oper[0].type != PROG_FLOAT)
        abort_interp("Non-float argument. (2)");
    if (!no_good(oper[0].data.fnumber) && !no_good(oper[1].data.fnumber)) {
        if (oper[1].data.fnumber < 0.0 &&
            oper[0].data.fnumber != floor(oper[0].data.fnumber)) {
            fresult = tp_alt_infinity_handler ? NAN : 0.0;
            fr->error.error_flags.imaginary = 1;
        } else {
            fresult = (double) pow(oper[1].data.fnumber, oper[0].data.fnumber);
        }
    } else {
        fresult = tp_alt_infinity_handler ? INF : 0.0;
        fr->error.error_flags.f_bounds = 1;
    }
    
    PushFloat(fresult);
}
void
prim_frand(PRIM_PROTOTYPE)
{
    double tresult, fresult;
	int result;
    
    result = rand();
    tresult = (double) rand();
    if ((result < tresult) && (result != tresult)) {
        fresult = result / tresult;
    } else {
        if (result != 0) {
            fresult = tresult / result;
        } else {
            fresult = 0.0;
            /* 0 is what we want here, no error.error */
        }
    }
    CHECKOFLOW(1);
    PushFloat(fresult);
}
void
prim_fmod(PRIM_PROTOTYPE)
{
	double fresult;
    
    if (oper[0].type == PROG_INTEGER) {
        oper[0].type = PROG_FLOAT;
        oper[0].data.fnumber = oper[0].data.number;
    }
    if (oper[1].type == PROG_INTEGER) {
        oper[1].type = PROG_FLOAT;
        oper[1].data.fnumber = oper[0].data.number;
    }
    if (oper[1].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    if (oper[0].type != PROG_FLOAT)
        abort_interp("Non-float argument. (2)");
    if (fabs(oper[0].data.fnumber) < DBL_EPSILON) {
        fresult = INF;
        fr->error.error_flags.div_zero = 1;
    } else {
        fresult = fmod(oper[1].data.fnumber, oper[0].data.fnumber);
    }
    
    PushFloat(fresult);
}
void
prim_modf(PRIM_PROTOTYPE)
{
    double tresult;
    double dresult, fresult;
    
    if (oper[0].type == PROG_INTEGER) {
        oper[0].type = PROG_FLOAT;
        oper[0].data.fnumber = oper[0].data.number;
    }
    if (oper[0].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    if (!no_good(oper[0].data.fnumber)) {
        fresult = modf(oper[0].data.fnumber, &dresult);
    } else {
        fresult = tp_alt_infinity_handler ? INF : 0.0;
        dresult = oper[0].data.fnumber;
        fr->error.error_flags.f_bounds = 1;
    }
    
    tresult = dresult;
    CHECKOFLOW(2);
    PushFloat(tresult);
    PushFloat(fresult);
}
void
prim_strtof(PRIM_PROTOTYPE)
{
	double fresult;
    
    if (oper[0].type != PROG_STRING)
        abort_interp("Non-string argument. (1)");
    fresult = 0.0;
    if (!oper[0].data.string || !ifloat(oper[0].data.string->data)) {
        fresult = 0.0;
        fr->error.error_flags.nan = 1;
    } else {
        sscanf(oper[0].data.string->data, "%lg", &fresult);
    }
    
    PushFloat(fresult);
}
void
prim_ftostr(PRIM_PROTOTYPE)
{
	char buf[BUFFER_LEN];
    if (oper[0].type == PROG_INTEGER) {
        oper[0].type = PROG_FLOAT;
        oper[0].data.fnumber = oper[0].data.number;
    }
    if (oper[0].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    sprintf(buf, "%#.15g", oper[0].data.fnumber);
    
    PushString(buf);
}
void
prim_ftostrc(PRIM_PROTOTYPE)
{
	char buf[BUFFER_LEN];
    if (oper[0].type == PROG_INTEGER) {
        oper[0].type = PROG_FLOAT;
        oper[0].data.fnumber = oper[0].data.number;
    }
    if (oper[0].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    sprintf(buf, "%.15g", oper[0].data.fnumber);
    
    PushString(buf);
}
void
prim_diff3(PRIM_PROTOTYPE)
{
    double xout, yout, zout;
    double x, y, z;
    double x2, y2, z2;
    if (oper[2].type != PROG_FLOAT)
        abort_interp("Non-float argument. (4)");
    if (oper[1].type != PROG_FLOAT)
        abort_interp("Non-float argument. (5)");
    if (oper[0].type != PROG_FLOAT)
        abort_interp("Non-float argument. (6)");
    x = oper[2].data.fnumber;
    y = oper[1].data.fnumber;
    z = oper[0].data.fnumber;
    if (oper[5].type != PROG_FLOAT)
        abort_interp("Non-float argument. (1)");
    if (oper[4].type != PROG_FLOAT)
        abort_interp("Non-float argument. (2)");
    if (oper[3].type != PROG_FLOAT)
        abort_interp("Non-float argument. (3)");
    x2 = oper[5].data.fnumber;
    y2 = oper[4].data.fnumber;
    z2 = oper[3].data.fnumber;
    xout = x - x2;
    yout = y - y2;
    zout = z - z2;
    PushFloat(xout);
    PushFloat(yout);
    PushFloat(zout);
}