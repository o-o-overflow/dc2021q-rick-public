# Rick Chall

Players found themselves in some weird 3D world. There are tons of levers which activate mysterious boxes following even more mysterious rules. You get the flag after winning N levels.

The chall has two components: the game (binary, C++) and the backend (a python script).

See below for how to setup a locally.


## Disclaimer

I wrote part of the opengl c++ codebase 10+ years ago for a class during my MS... and the rest has been written under a lot of pressure... I'm not exactly proud of the quality of this codebase ;-) But, still, it did its job, and this is anyways a good starting point to mess with the chall and/or to get an idea about opengl code, regardless of the CTF chall. Enjoy & don't judge (too much)!

## Notes for local setup

- setup the backend:
  - Run `cd service && ./run-backend-docker.sh`
  - The script will build + run the docker container with `backend.py`, and it will print the container IP. The backend should be now reachable with `nc <IP> 4343`. You should get back `RICK` + some random bytes.

- build the game / binary:
  - Run `cd service && ./make-docker.sh`. This script will build + run the container for creating the binary. If everything goes well, you will find the binary at `./service/rick`. This `rick` binary is self-contained and already embeds the various resources. 

- patch the game so that it reaches the proper backend:
  - the game reaches out to `rick.challenges.ooo`. The backend will stay up for a while, but if you want to play with this locally, you may want to patch your `/etc/hosts` so that this domain name points to the IP of the backend's container.

- install the dependencies to run the game:
  - `sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev` should be enough
  - tested on Ubuntu 20.04.
