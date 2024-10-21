set -xm

gcc main.c -o sift -Werror -Wextra
sudo mv sift /usr/bin