# To update the docker image on github registry

## Build the image locally

docker build -t ghcr.io/raphaelmartin83/memoto_linux_<arch> - < Dockerfile

## Create your token on github

Profile / Settings / Developer Settings / Personnal access tokens / Tokens (classic) / Generate new token

## Log in from your terminal

export CR_PAT=TheGivenToken
echo $CR_PAT | docker login ghcr.io -u RaphaelMartin83 --password-stdin

## Push!

docker push ghcr.io/raphaelmartin83/memoto_linux_<arch>

# To run the docker image

docker run -e OUTPUT_DIR=$APPIMAGE_WRITE_DIR -v $MEMOTO_REPO:/var/mmt -it ghcr.io/raphaelmartin83/memoto_linux_<arch>
