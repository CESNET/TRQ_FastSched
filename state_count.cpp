/*
*         OpenPBS (Portable Batch System) v2.3 Software License
*
* Copyright (c) 1999-2000 Veridian Information Solutions, Inc.
* All rights reserved.
*
* ---------------------------------------------------------------------------
* For a license to use or redistribute the OpenPBS software under conditions
* other than those described below, or to purchase support for this software,
* please contact Veridian Systems, PBS Products Department ("Licensor") at:
*
*    www.OpenPBS.org  +1 650 967-4675                  sales@OpenPBS.org
*                        877 902-4PBS (US toll-free)
* ---------------------------------------------------------------------------
*
* This license covers use of the OpenPBS v2.3 software (the "Software") at
* your site or location, and, for certain users, redistribution of the
* Software to other sites and locations.  Use and redistribution of
* OpenPBS v2.3 in source and binary forms, with or without modification,
* are permitted provided that all of the following conditions are met.
* After December 31, 2001, only conditions 3-6 must be met:
*
* 1. Commercial and/or non-commercial use of the Software is permitted
*    provided a current software registration is on file at www.OpenPBS.org.
*    If use of this software contributes to a publication, product, or
*    service, proper attribution must be given; see www.OpenPBS.org/credit.html
*
* 2. Redistribution in any form is only permitted for non-commercial,
*    non-profit purposes.  There can be no charge for the Software or any
*    software incorporating the Software.  Further, there can be no
*    expectation of revenue generated as a consequence of redistributing
*    the Software.
*
* 3. Any Redistribution of source code must retain the above copyright notice
*    and the acknowledgment contained in paragraph 6, this list of conditions
*    and the disclaimer contained in paragraph 7.
*
* 4. Any Redistribution in binary form must reproduce the above copyright
*    notice and the acknowledgment contained in paragraph 6, this list of
*    conditions and the disclaimer contained in paragraph 7 in the
*    documentation and/or other materials provided with the distribution.
*
* 5. Redistributions in any form must be accompanied by information on how to
*    obtain complete source code for the OpenPBS software and any
*    modifications and/or additions to the OpenPBS software.  The source code
*    must either be included in the distribution or be available for no more
*    than the cost of distribution plus a nominal fee, and all modifications
*    and additions to the Software must be freely redistributable by any party
*    (including Licensor) without restriction.
*
* 6. All advertising materials mentioning features or use of the Software must
*    display the following acknowledgment:
*
*     "This product includes software developed by NASA Ames Research Center,
*     Lawrence Livermore National Laboratory, and Veridian Information
*     Solutions, Inc.
*     Visit www.OpenPBS.org for OpenPBS software support,
*     products, and information."
*
* 7. DISCLAIMER OF WARRANTY
*
* THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND. ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT
* ARE EXPRESSLY DISCLAIMED.
*
* IN NO EVENT SHALL VERIDIAN CORPORATION, ITS AFFILIATED COMPANIES, OR THE
* U.S. GOVERNMENT OR ANY OF ITS AGENCIES BE LIABLE FOR ANY DIRECT OR INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* This license will be governed by the laws of the Commonwealth of Virginia,
* without reference to its choice of law rules.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "torque.h"
#include "state_count.h"
#include "constant.h"
#include "misc.h"


using namespace std;

/*
 *
 * print_state_count - print out a state_count struct
 *
 *   sc - the struct to print
 *
 * returns nothing
 *
 */
void print_state_count(state_count *sc)
  {
  printf("running: %d\n", sc -> running);
  printf("queued: %d\n", sc -> queued);
  printf("transit: %d\n", sc -> transit);
  printf("exiting: %d\n", sc -> exiting);
  printf("held: %d\n", sc -> held);
  printf("waiting: %d\n", sc -> waiting);
  printf("suspended: %d\n", sc -> suspended);
  printf("completed: %d\n", sc -> completed);
  printf("total: %d\n", sc -> total);
  }

/*
 *
 * init_state_count - initalize state count struct
 *
 *   sc - the struct to initalize
 *
 * returns nothing
 *
 */
void init_state_count(state_count *sc)
  {
  sc -> running = 0;
  sc -> queued = 0;
  sc -> transit = 0;
  sc -> exiting = 0;
  sc -> held = 0;
  sc -> waiting = 0;
  sc -> suspended = 0;
  sc -> completed = 0;
  sc -> crossrun = 0;
  sc -> total = 0;
  }

void count_states(const vector<JobInfo*>& jobs, state_count& sc)
  {
  for (auto j : jobs)
    {
    switch (j->state)
      {
      case JobQueued:     sc.queued++;     break;
      case JobRunning:    sc.running++;    break;
      case JobTransit:    sc.transit++;    break;
      case JobExiting:    sc.exiting++;    break;
      case JobHeld:       sc.held++;       break;
      case JobWaiting:    sc.waiting++;    break;
      case JobSuspended:  sc.suspended++;  break;
      case JobCompleted:  sc.completed++;  break;
      case JobCrossRun:   sc.crossrun++;   break;
      default:
        sched_log(PBSEVENT_JOB, PBS_EVENTCLASS_JOB, j->job_id.c_str(), "Job in unknown state");
      }
    }

  sc.total = sc.queued + sc.running + sc.transit + sc.exiting + sc.held + sc.waiting + sc.suspended + sc.completed + sc.crossrun;
  }


void count_states(const vector<JobInfo*>& jobs, state_count *sc)
  {
  count_states(jobs.data(),sc);
  }
/*
 *
 * count_states - count the jobs in each state and set the state counts
 *
 *   jobs - array of jobs
 *   sc   - state count structure passed by reference
 *
 * returns nothing
 *
 */
void count_states(JobInfo * const *jobs, state_count *sc)
  {
  int i;

  if (jobs  != NULL)
    {
    for (i = 0; jobs[i] != NULL; i++)
      {
      switch (jobs[i]->state)
        {
        case JobQueued:     sc->queued++;     break;
        case JobRunning:    sc->running++;    break;
        case JobTransit:    sc->transit++;    break;
        case JobExiting:    sc->exiting++;    break;
        case JobHeld:       sc->held++;       break;
        case JobWaiting:    sc->waiting++;    break;
        case JobSuspended:  sc->suspended++;  break;
        case JobCompleted:  sc->completed++;  break;
        case JobCrossRun:   sc->crossrun++;   break;
        default:
          sched_log(PBSEVENT_JOB, PBS_EVENTCLASS_JOB, jobs[i] -> job_id.c_str(), "Job in unknown state");
        }
      }
    }

  sc -> total = sc -> queued + sc -> running + sc -> transit + sc -> exiting + sc -> held + sc -> waiting + sc -> suspended + sc -> completed + sc -> crossrun;

  }

/*
 *
 * total_states - add the states from sc2 to the states in sc1
 *         i.e. sc1 += sc2
 *
 *   sc1 - the accumlator
 *   sc2 - what is being totaled
 *
 * returns nothing
 *
 */
void total_states(state_count *sc1, state_count *sc2)
  {
  sc1 -> running += sc2 -> running;
  sc1 -> queued += sc2 -> queued;
  sc1 -> held += sc2 -> held;
  sc1 -> waiting += sc2 -> waiting;
  sc1 -> exiting += sc2 -> exiting;
  sc1 -> transit += sc2 -> transit;
  sc1 -> suspended += sc2 -> suspended;
  sc1 -> completed += sc2 -> completed;
  sc1 -> crossrun += sc2 -> crossrun;
  sc1 -> total += sc2 -> total;
  }

