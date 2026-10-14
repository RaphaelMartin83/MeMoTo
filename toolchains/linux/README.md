# To update the docker image on github registry

Build the Qt toolchain image for the machine's native architecture:

```sh
docker build -t ghcr.io/raphaelmartin83/qt_compilation_base -f toolchains/Dockerfile toolchains/
```

Then build the matching Linux image (use `x86_64` on an x64 machine or `arm64` on an ARM64 machine):

```sh
docker build -t ghcr.io/raphaelmartin83/memoto_linux_<arch> -f toolchains/linux/<arch>/Dockerfile .
```

## Publish an image to GitHub Container Registry

Create a GitHub personal access token with package write access and log in:

```sh
echo "$CR_PAT" | docker login ghcr.io -u RaphaelMartin83 --password-stdin
```

Then push the image:

```sh
docker push ghcr.io/raphaelmartin83/qt_compilation_base
docker push ghcr.io/raphaelmartin83/memoto_linux_<arch>
```

# To run the docker image

Run the matching image on its native architecture:

```sh
docker run -e OUTPUT_DIR=$APPIMAGE_WRITE_DIR -v $MEMOTO_REPO:/var/mmt -it ghcr.io/raphaelmartin83/memoto_linux_<arch>
```
