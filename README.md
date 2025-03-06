# Abelian-Sandpile
Console app that builds Abelian sandpile model with no limits depending on users input.
user chooses:
1) flag -i directory where .tsv file with input data locates
2) flag -f freaquency of saving intermediate sandpiles (default: only saves finished one)
3) flag -m maximum
4) flag -o directory where .bmp files are going to be saved

.tsv input file format:
- x_coordinate  y_coordinate number_of_sand_grains
- x_coordinate  y_coordinate number_of_sand_grains
...
(all separated by tabs)

All project done without using std containers in educational purposes
