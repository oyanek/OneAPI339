# Word Cloud

## Configuring

To run this project please follow the steps bellow.
- connect to Devcloud via ssh
- clone project into directory of your choosing

```bash
$ qsub -I -l nodes=1:gpu:ppn=2 -d .
#cd into /OneApi339
$ cmake -S . -B build
$ cmake --build build
