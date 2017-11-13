/*
 * tiny_ekf_struct.h: common data structure for TinyEKF
 *
 * You should #include this file after using #define for N (states) and M
*  (observations)
 *
 * Copyright (C) 2016 Simon D. Levy
 *
 * This code is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this code.  If not, see <http:#www.gnu.org/licenses/>.
 */

typedef struct {

    int n;          /* number of state values */
    int m;          /* number of observables */
    int s;          /* number of input values */

    double x[Nsta];    /* state vector */

    double P[Nsta][Nsta];  /* prediction error covariance */
    double Y[Sinp][Sinp];  /* input noise covariance */
    double Q[Nsta][Nsta];  /* process noise covariance */
    double R[Mobs][Mobs];  /* measurement error covariance */

    double G[Nsta][Mobs];  /* Kalman gain; a.k.a. K */

    double F[Nsta][Nsta];  /* Jacobian of process model */
    double L[Nsta][Sinp];  /* Jacobian of input model */
    double H[Mobs][Nsta];  /* Jacobian of measurement model */

    double Ht[Nsta][Mobs]; /* transpose of measurement Jacobian */
    double Lt[Sinp][Nsta]; /* transpose of input Jacobian */
    double Ft[Nsta][Nsta]; /* transpose of process Jacobian */
    double Pp[Nsta][Nsta]; /* P, post-prediction, pre-update */

    double fx[Nsta];   /* output of user defined f() state-transition function */
    double hx[Mobs];   /* output of user defined h() measurement function */

    /* temporary storage */
    double tmp0[Nsta][Nsta];
    double tmp1[Nsta][Mobs];
    double tmp2[Mobs][Nsta];
    double tmp3[Mobs][Mobs];
    double tmp4[Mobs][Mobs];
    double tmp5[Mobs]; 
    double tmp6[Nsta][Sinp];

} ekf_t;        
