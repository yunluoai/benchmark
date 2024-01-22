/* BEEBS cubic benchmark

   Contributor: James Pallister <james.pallister@bristol.ac.uk>

   This file is part of Embench and was formerly part of the Bristol/Embecosm
   Embedded Benchmark Suite.

   SPDX-License-Identifier: GPL-3.0-or-later */

#define RPT 3

#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef PI
#define PI (4 * atan(1))
#endif

/* Common understanding of a "small value" (epsilon) for floating point
   comparisons. */

#define VERIFY_DOUBLE_EPS 1.0e-13

#define double_eq_beebs(exp, actual) (fabs(exp - actual) < VERIFY_DOUBLE_EPS)

void SolveCubic(double a, double b, double c, double d, int *solutions, double *x)
{
  long double a1 = (long double)(b / a);
  long double a2 = (long double)(c / a);
  long double a3 = (long double)(d / a);
  long double Q = (a1 * a1 - 3.0L * a2) / 9.0L;
  long double R = (2.0L * a1 * a1 * a1 - 9.0L * a1 * a2 + 27.0L * a3) / 54.0L;
  double R2_Q3 = (double)(R * R - Q * Q * Q);

  double theta;

  if (R2_Q3 <= 0)
  {
    *solutions = 3;
    theta = acos(((double)R) / sqrt((double)(Q * Q * Q)));
    x[0] = -2.0 * sqrt((double)Q) * cos(theta / 3.0) - a1 / 3.0;
    x[1] =
        -2.0 * sqrt((double)Q) * cos((theta + 2.0 * PI) / 3.0) - a1 / 3.0;
    x[2] =
        -2.0 * sqrt((double)Q) * cos((theta + 4.0 * PI) / 3.0) - a1 / 3.0;
  }
  else
  {
    *solutions = 1;
    x[0] = pow(sqrt(R2_Q3) + fabs((double)R), 1 / 3.0);
    x[0] += ((double)Q) / x[0];
    x[0] *= (R < 0.0L) ? 1 : -1;
    x[0] -= (double)(a1 / 3.0L);
  }
}

/* ------------------------------ main ------------------------------ */

int main()
{

  int soln_cnt0, soln_cnt1;
  double res0[3];
  double res1;
  int correct, res, i;

  for (i = 0; i < RPT; i++)
  {
    double a1 = 1.0, b1 = -10.5, c1 = 32.0, d1 = -30.0;
    double a2 = 1.0, b2 = -4.5, c2 = 17.0, d2 = -30.0;
    double a3 = 1.0, b3 = -3.5, c3 = 22.0, d3 = -31.0;
    double a4 = 1.0, b4 = -13.7, c4 = 1.0, d4 = -35.0;
    int solutions;

    double output[48] = {0};
    double *output_pos = &(output[0]);

    /* solve some cubic functions */
    /* should get 3 solutions: 2, 6 & 2.5   */
    SolveCubic(a1, b1, c1, d1, &solutions, output);
    soln_cnt0 = solutions;
    memcpy(res0, output, 3 * sizeof(res0[0]));
    /* should get 1 solution: 2.5           */
    SolveCubic(a2, b2, c2, d2, &solutions, output);
    soln_cnt1 = solutions;
    res1 = output[0];
    SolveCubic(a3, b3, c3, d3, &solutions, output);
    SolveCubic(a4, b4, c4, d4, &solutions, output);
    /* Now solve some random equations */
    for (a1 = 1; a1 < 3; a1++)
    {
      for (b1 = 10; b1 > 8; b1--)
      {
        for (c1 = 5; c1 < 6; c1 += 0.5)
        {
          for (d1 = -1; d1 > -3; d1--)
          {
            SolveCubic(a1, b1, c1, d1, &solutions, output_pos);
          }
        }
      }
    }
  }

  const double exp_res0[3] = {2.0, 6.0, 2.5};
  const double exp_res1 = 2.5;
  res = (3 == soln_cnt0) && double_eq_beebs(exp_res0[0], res0[0]) && double_eq_beebs(exp_res0[1], res0[1]) && double_eq_beebs(exp_res0[2], res0[2]) && (1 == soln_cnt1) && double_eq_beebs(exp_res1, res1);

  correct = res == 1 ? 1 : 0;

  printf("The result is: %d\n", correct);

  return 0;
}

/*
   Local Variables:
   mode: C
   c-file-style: "gnu"
   End:
*/
