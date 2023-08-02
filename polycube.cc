#include <stdlib.h>
#include <stdint.h>
#include <strings.h>
#include <assert.h>
#include <string>
#include <cstring>
#include <unordered_set>
#include <algorithm>
#include <iostream>

#define CHUNKS 128

#define PERMS 48
static const uint8_t perm[PERMS][3] =
{
  { 0, 2, 4 }, //  x,  y,  z
  { 2, 1, 4 }, //  y, -x,  z
  { 1, 3, 4 }, // -x, -y,  z
  { 3, 0, 4 }, // -y,  x,  z
  { 2, 0, 5 }, //  y,  x, -z
  { 0, 3, 5 }, //  x, -y, -z
  { 3, 1, 5 }, // -y, -x, -z
  { 1, 2, 5 }, // -x,  y, -z
  { 4, 0, 2 }, //  z,  x,  y
  { 0, 5, 2 }, //  x, -z,  y
  { 5, 1, 2 }, // -z, -x,  y
  { 1, 4, 2 }, // -x,  z,  y
  { 0, 4, 3 }, //  x,  z, -y
  { 4, 1, 3 }, //  z, -x, -y
  { 1, 5, 3 }, // -x, -z, -y
  { 5, 0, 3 }, // -z,  x, -y
  { 2, 4, 0 }, //  y,  z,  x
  { 4, 3, 0 }, //  z, -y,  x
  { 3, 5, 0 }, // -y, -z,  x
  { 5, 2, 0 }, // -z,  y,  x
  { 4, 2, 1 }, //  z,  y, -x
  { 2, 5, 1 }, //  y, -z, -x
  { 5, 3, 1 }, // -z, -y, -x
  { 3, 4, 1 }, // -y,  z, -x
  { 0, 2, 5 }, //  x,  y, -z
  { 2, 1, 5 }, //  y, -x, -z
  { 1, 3, 5 }, // -x, -y, -z
  { 3, 0, 5 }, // -y,  x, -z
  { 2, 0, 4 }, //  y,  x,  z
  { 0, 3, 4 }, //  x, -y,  z
  { 3, 1, 4 }, // -y, -x,  z
  { 1, 2, 4 }, // -x,  y,  z
  { 4, 0, 3 }, //  z,  x, -y
  { 0, 5, 3 }, //  x, -z, -y
  { 5, 1, 3 }, // -z, -x, -y
  { 1, 4, 3 }, // -x,  z, -y
  { 0, 4, 2 }, //  x,  z,  y
  { 4, 1, 2 }, //  z, -x,  y
  { 1, 5, 2 }, // -x, -z,  y
  { 5, 0, 2 }, // -z,  x,  y
  { 2, 4, 1 }, //  y,  z, -x
  { 4, 3, 1 }, //  z, -y, -x
  { 3, 5, 1 }, // -y, -z, -x
  { 5, 2, 1 }, // -z,  y, -x
  { 4, 2, 0 }, //  z,  y,  x
  { 2, 5, 0 }, //  y, -z,  x
  { 5, 3, 0 }, // -z, -y,  x
  { 3, 4, 0 }, // -y,  z,  x
};

//******************************************************************************

void eval(std::unordered_set<std::string> &store, int n, const char *str)
{
  // fill bits
  uint8_t bits[n + 2][n + 2][n + 2];
  bzero(bits, (n + 2) * (n + 2) * (n + 2));
  int size_x = 0;
  int size_y = 0;
  int size_z = 0;
  for (int i = 0; i < n; i++)
  {
    int x = str[4 * i + 0] - 'A' + 1;
    if (x > size_x)
      size_x = x;
    int y = str[4 * i + 1] - 'A' + 1;
    if (y > size_y)
      size_y = y;
    int z = str[4 * i + 2] - 'A' + 1;
    if (z > size_z)
      size_z = z;
    bits[x][y][z] = 1;
  }

  // loop every extension
  for (int x = 0; x <= size_x + 1; x++)
    for (int y = 0; y <= size_y + 1; y++)
      for (int z = 0; z <= size_z + 1; z++)
	if (!bits[x][y][z]
	  && ((x > 0 && bits[x - 1][y][z])
	    || (x <= size_x && bits[x + 1][y][z])
	    || (y > 0 && bits[x][y - 1][z])
	    || (y <= size_y && bits[x][y + 1][z])
	    || (z > 0 && bits[x][y][z - 1])
	    || (z <= size_z && bits[x][y][z + 1])))
	{
	  bits[x][y][z] = 1;

	  // code extension in all symeries
	  int min_x = x < 1 ? 0 : 1;
	  int max_x = x > size_x ? x : size_x;
	  int min_y = y < 1 ? 0 : 1;
	  int max_y = y > size_y ? y : size_y;
	  int min_z = z < 1 ? 0 : 1;
	  int max_z = z > size_z ? z : size_z;
	  
//******************************************************************************

	  uint32_t code[PERMS][n + 1];
	  int i = 0;
	  for (int x = min_x; x <= max_x; x++)
	    for (int y = min_y; y <= max_y; y++)
	      for (int z = min_z; z <= max_z; z++)
		if (bits[x][y][z])
		{
		  //               0 -> x    1 -> -x   2 -> y    3 -> -y   4 -> z    5 -> -z
		  int coord[6] = { x - min_x, max_x - x, y - min_y, max_y - y, z - min_z, max_z - z };

		  for (int p = 0; p < PERMS; p++)
		  {
		    int a = coord[perm[p][0]];
		    int b = coord[perm[p][1]];
		    int c = coord[perm[p][2]];
		    code[p][i] = (a << 16) | (b << 8) | (c << 0);
		  }
		  i++;
		}

  //******************************************************************************

	  std::string smallestRotation("Z");
	  std::string smallestReflection("Z");
	  int size[6] = { max_x - min_x, max_x - min_x,
	    max_y - min_y, max_y - min_y, max_z - min_z, max_z - min_z };
	  for (int p = 0; p < PERMS; p++)
	  {
	    if (size[perm[p][0]] > size[perm[p][1]] || size[perm[p][1]] > size[perm[p][2]])
	      continue;

	    std::sort(&code[p][0], &code[p][n + 1]);

	    std::string key(4 * i - 1, ' ');
	    for (int i = 0; i < n + 1; i++)
	    {
	      key[4 * i + 0] = ((code[p][i] >> 16) & 0xff) + 'A';
	      key[4 * i + 1] = ((code[p][i] >> 8) & 0xff) + 'A';
	      key[4 * i + 2] = ((code[p][i] >> 0) & 0xff) + 'A';
	    }

	    if (p < PERMS / 2 && key < smallestRotation)
	      smallestRotation = key;

	    if (key < smallestReflection)
	      smallestReflection = key;
	  }

	  std::string printout(smallestRotation);
	  if (smallestRotation > smallestReflection)
	    printout += " =";

#pragma omp critical
	  {
	    auto inserted = store.insert(smallestRotation);
	    if (inserted.second) // inserted
	      std::cout << printout << "\n";
	  }

	  bits[x][y][z] = 0;
	}
}

//******************************************************************************

int main(int ac, char *av[])
{
  assert(ac == 2);
  int n = atoi(av[1]);
  assert(n > 0);

  std::unordered_set<std::string> store;

  char str[CHUNKS][4 * n + 4];
  for (size_t chunks = CHUNKS; chunks == CHUNKS;)
  {
    for (chunks = 0; chunks < CHUNKS; chunks++)
    {
      char *read = fgets(str[chunks], 4 * n + 3, stdin);
      if (!read)
	break;
      assert(str[chunks][4 * n - 1] == '\n' || str[chunks][4 * n + 1] == '\n');
    }

#pragma omp parallel for schedule(runtime)
    for (size_t idx = 0; idx < chunks; idx++)
      eval(store, n, str[idx]);
  }

  return EXIT_SUCCESS;
}

//******************************************************************************
