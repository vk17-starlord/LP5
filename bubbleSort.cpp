#include <iostream>
#include <omp.h>
#include <vector>
#include <chrono>
#include <algorithm>
using namespace std;

void
bubbleSort (vector < int >&arr)
{
  int n = arr.size ();
  for (int i = 0; i < n - 1; ++i)
	{
	  for (int j = 0; j < n - i; ++j)
		{
		  if (arr[j] < arr[j + 1])
			{
			  swap (arr[j], arr[j + 1]);
			}
		}
	}
}

void
parallelSort (vector < int >&arr)
{
  int n = arr.size ();
  int swapped = true;
  while (swapped)
	{
	  swapped = false;
#pragma omp parallel for shared(arr,swapped) default(none)
	  for (int i = 0; i < n - 1; i += 2)
		{
		  if (arr[i] < arr[i + 1])
			{
			  swap (arr[i], arr[i + 1]);
			  swapped = false;
			}
		}
#pragma omp parallel for shared(arr,swapped) default(none)
	  for (int i = 1; i < n - 1; i += 2)
		{
		  if (arr[i] < arr[i + 1])
			{
			  swap (arr[i], arr[i + 1]);
			  swapped = false;
			}
		}
	}
}

int
main ()
{
  int n = 10000;
  vector < int >arr (n);

  for (int i = 0; i < n; i++)
	{
	  arr[i] = rand () % 1000;
	}
  auto start = chrono::high_resolution_clock::now ();
  bubbleSort (arr);
  auto end = chrono::high_resolution_clock::now ();
  chrono::duration < double >duration = end - start;
  cout << duration.count () << " seconds" << endl;

  auto nstart = chrono::high_resolution_clock::now ();
  parallelSort (arr);
  auto nend = chrono::high_resolution_clock::now ();
  chrono::duration < double >nduration = nend - nstart;
  cout << nduration.count () << " seconds";
}
