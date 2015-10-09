# Madeline_2.0_PDE

[![Build Status](https://travis-ci.org/piratical/Madeline_2.0_PDE.svg)](https://travis-ci.org/piratical/Madeline_2.0_PDE)

[Build Artifacts](http://usf-hii-madeline-ci.s3-website-us-west-2.amazonaws.com/)


## INTRODUCTION

The Madeline 2.0 Pedigree Drawing Engine (PDE) is a pedigree drawing program designed to handle large and
complex pedigrees with an emphasis on readability and aesthetics. The program was designed primarily for
human pedigrees, although people have also used Madeline for animal and controlled breeding projects.

Although the program takes inspiration from Madeline v. 0.935 written by Ed Trager, the Madeline 2.0 Pedigree
Drawing Engine, or just Madeline 2.0 for short, is in fact a completely new and different program written from
scratch by Ed Trager, Ritu Khanna, and Adrian Marrs, with additional contributions by a few other people.

For complex pedigrees we use a hybrid algorithm in which consanguinous loops are drawn as cyclic graphs whenever
possible, but we resort to acyclic graphs when matings can no longer be connected without line crossings. A similar
hybrid approach is used to avoid line crossings for matings between far-flung descendants of different founding
groups.

Written in object-oriented C++ and released under the GNU General Public License (GPL), Madeline 2.0 reads input
files specified on the command line and generates pedigree drawings without user interaction. Pedigree output in
scalable vector graphics (SVG) format can be viewed in all browsers with native SVG rendering support.

The paper describing our work was published in the journal Bioinformatics in 2007:

     Madeline 2.0 PDE: A new program for local and web-based pedigree drawing
     Edward H. Trager; Ritu Khanna; Adrian Marrs; Lawrence Siden; Kari E.H. Branham; Anand Swaroop; Julia E. Richards
     Bioinformatics (2007) 23 (14): 1854-1856.
     doi: 10.1093/bioinformatics/btm242

     http://bioinformatics.oxfordjournals.org/content/23/14/1854.abstract?keytype=ref&ijkey=2YnVbuTikoP76f6

Our dedicated web site with extensive documentation, test data, and a demonstration interactive web application
may be found at [https://madeline.med.umich.edu/madeline/](https://madeline.med.umich.edu/madeline/).

## A BRIEF HISTORY

Sometime around 1996 Ed Trager began to writing a program for preparing, visualizing, and exploring human pedigree
data used in genetic linkage studies. This became the program known simply as "Madeline."

In addition to converting pedigree and marker data into various formats required by linkage analysis software,
including Crimap, Genehunter, Allegro, Mendel, Merlin, PedCheck, and Simwalk2, the original version of Madeline
also provided functionality for querying pedigree data sets and for drawing pedigrees. Development of the original
version of Madeline reached version 0.936, while the stable version was 0.935. Although Madeline 0.935 is a capable
program, it's pedigree drawing routines have many limitations because the program was originally only designed to
handle single descent trees of simple nuclear families. Consanguinous loops, multiple mated individuals, and multiple
founding groups often occur in the extended family pedigrees used in modern genetic studies, and much work has been
invested in finding better ways to visualize these kinds of pedigrees.

In this new program, Madeline 2.0 Pedigree Drawing Engine, we have tried to create a much more robust pedigree
drawing engine to address the limitations of the older program and to provide the worldwide research community with
a better tool for pedgiree visualization.  We hope you will enjoy using our tool and report bugs and suggestions
for improvement to us.

---

— Ed Trager,
  Principal author, Madeline 2.0 PDE
  Kellogg Eye Center
  University of Michigan, Ann Arbor
  2015.09.23


