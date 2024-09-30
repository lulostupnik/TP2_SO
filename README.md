
# TP2_SO Compilation Guide

Follow these steps to clone the repository, set up the environment, and run the project.

## Requirements
Make sure you have the following software installed on your system:
- [QEMU](https://www.qemu.org/): Specifically, `qemu-system-x86_64` is required.

## Table of Contents
- [Step 1: Pull the Docker Image](#step-1-pull-the-docker-image)
- [Step 2: Clone the Repository](#step-2-clone-the-repository)
- [Step 3: Navigate to the Project Directory](#step-3-navigate-to-the-project-directory)
- [Step 4: Set Up the Docker Environment](#step-4-set-up-the-docker-environment)
- [Step 5: Exit the Docker Container](#step-5-exit-the-docker-container)
- [Step 6: Run the Project](#step-6-run-the-project)

## Step 1: Pull the Docker Image
Start by pulling the Docker image. Open your terminal and run:
```sh
docker pull agodio/itba-so-multi-platform:3.0
```

## Step 2: Clone the Repository
Clone the repository from GitHub. Open your terminal and run:
```sh
git clone git@github.com:lulostupnik/TP2_SO.git
```

## Step 3: Navigate to the Project Directory
Once the repository is cloned, move into the project directory:
```sh
cd TP2_SO
```

## Step 4: Set Up the Docker Environment
Use Docker to set up a multi-platform environment. Run the following command to start a Docker container:
```sh
docker run -v ${PWD}:/root --privileged -ti --name SO agodio/itba-so-multi-platform:3.0
```


## Step 5: Exit the Docker Container

```sh
exit
```

## Step 6: Run the Project
Finally, run the project by executing the following script:
```sh
./run.sh
```
