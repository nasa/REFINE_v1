
/* Michael A. Park
 * Computational Modeling & Simulation Branch
 * NASA Langley Research Center
 * Phone:(757)864-6604
 * Email:m.a.park@larc.nasa.gov 
 */
  
/* $Id$ */

#include <stdlib.h>
#include <check.h>
#include "sampleunit.h"

START_TEST(sampleunit_test)
{
  fail_unless( sampleunit(1,2) == 3,
	       "sample unit not returning the expected 1+2=3");
}
END_TEST

Suite *sampleunit_suite (void) 
{ 
  Suite *s = suite_create ("SampleUnit"); 
  TCase *tc_core = tcase_create ("Core");
 
  suite_add_tcase (s, tc_core);
 
  tcase_add_test (tc_core, sampleunit_test); 
  return s; 
}
 
int main (void) 
{ 
  int nf; 
  Suite *s = sampleunit_suite (); 
  SRunner *sr = srunner_create (s); 
  srunner_run_all (sr, CK_NORMAL); 
  nf = srunner_ntests_failed (sr); 
  srunner_free (sr); 
  suite_free (s); 
  return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE; 
}

