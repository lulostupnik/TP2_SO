
# TP2_SO Compilation Guide

Follow these steps to clone the repository, set up the environment, and run the project.

## Table of Contents
- [Step 1: Clone the Repository](#step-1-clone-the-repository)
- [Step 2: Navigate to the Project Directory](#step-2-navigate-to-the-project-directory)
- [Step 3: Set Up the Docker Environment](#step-3-set-up-the-docker-environment)
- [Step 4: Exit the Docker Container](#step-4-exit-the-docker-container)
- [Step 5: Run the Project](#step-5-run-the-project)

## Step 1: Clone the Repository
Start by cloning the repository from GitHub. Open your terminal and run:
```sh
git clone git@github.com:lulostupnik/TP2_SO.git
```

## Step 2: Navigate to the Project Directory
Once the repository is cloned, move into the project directory:
```sh
cd TP2_SO
```

## Step 3: Set Up the Docker Environment
Use Docker to set up a multi-platform environment. Run the following command to start a Docker container:
```sh
docker run -v ${PWD}:/root --privileged -ti --name SO agodio/itba-so-multi-platform:3.0
```

## Step 4: Exit the Docker Container

```sh
exit
```

## Step 5: Run the Project
Finally, run the project by executing the following script:
```sh
./run.sh
```

