POLYCUBE
========

[A polycube is a solid figure formed by joining one or more equal cubes face to face.](https://en.wikipedia.org/wiki/Polycube)

Determine the number of different polycubes constructed from *n* cubes wrt.
* rotation - OEIS sequence [A000162](https://oeis.org/A000162)
* rotation and reflection - OEIS sequence [A038119](https://oeis.org/A038119)
The later puts polycubes that differ only in chirality into the same equivalence class. 

Algorithmic Ideas
-----------------

Enumerate all equivalence classes of *n*-polycubes based on the 
equivalence classes of *n-1*-polycubes starting with the
equivalence class of the monocube.

* The slow algorithm: Enumerate all extensions of one representative
  of the *n-1*-polycube equivalence classes, hash all rotations (and
  reflections) so that new candidates can be checked quickly.
  This approach is slow, does not scale at all on multiple processors,
  and consumes much memory, which limits the size of *n*
* Calculating all rotations (and reflections) for every candidate,
  find the lexicographic smallest representation, and hash only
  this instance. It turns out to be twice as fast, parallelizing
  linearly.
* For *n* where no hash fits into memory the lexicographic smallest
  representation of each enumerated extension can be written to a file,
  which can be unique-sorted afterwards

Computational Results
---------------------

| n  | A000162     | A038119     | Intel Core i7-9700K |
| -- | ----------- | ----------- | ------------------- |
| 01 |           1 |           1 |                     |
| 02 |           1 |           1 |                     |
| 03 |           2 |           2 |                     |
| 04 |           8 |           7 |                     |
| 05 |          29 |          23 |                     |
| 06 |         166 |         112 |                     |
| 07 |        1023 |         607 |                     |
| 08 |        6922 |        3811 |                     |
| 09 |       48311 |       25413 |                0.1s |
| 10 |      346543 |      178083 |                0.6s |
| 11 |     2522522 |     1279537 |                5.5s |
| 12 |    18598427 |     9371094 |               49.7s |
| 13 |   138462649 |    69513546 |              449.9s |
| 14 |  1039496297 |   520878101 |                     |
| 15 |  7859514470 |  3934285874 |                     |
| 16 | 59795121480 | 29915913663 |                     |

File Format
-----------

* one polycube per line
* the coordinates of the cubes given ar triplet of letters
* the lexiograhic smallest representation is chosen

For example the monocube is written as
```
AAA
```

The smallest representation for the dicube is
```
AAA AAB
```

Starting with *n*=4 

For example, 6 tetracubes have mirror symmetry and one is chiral, giving a count of 7 or 8 tetracubes respectively
```
AAA AAB AAC AAD
AAA AAB AAC ABA
AAA AAB AAC ABB
AAA AAB ABA ABB
AAA AAB ABA BAA
AAA AAB ABA BAB
AAA AAB ABA BBA =
AAA AAB ABB ABC
```
