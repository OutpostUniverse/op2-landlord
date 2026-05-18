# For building on GitHub Actions

# To update:
# 1. Bump the version number tag on `imageName` in: `.github/workflows/buildDockerBuildEnv.yml`
# 2. Make and push changes to Dockerfile
# 3. Run `buildDockerBuildEnv` workflow on branch with changes
#      https://github.com/OutpostUniverse/OP2Utility/actions/workflows/buildDockerBuildEnv.yml
# 4. Update `.github/workflows/build.yml` workflow to use new version number tag

FROM ghcr.io/outpostuniverse/op2utility:1.2

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    nlohmann-json3-dev \
    libimgui-dev \
  && rm -rf /var/lib/apt/lists/*
