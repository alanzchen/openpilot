# syntax=docker/dockerfile:1
# prepare_rootfs.py verifies AGNOS sparse/raw hashes before creating this context.
# Keep the actual device userspace: no Ubuntu base image or apt upgrades.
FROM scratch
ADD agnos-rootfs.tar /
COPY pins.json provenance.json uv.lock bootstrap.py /opt/linux-device-build/
ENV VIRTUAL_ENV=/usr/local/venv \
    PATH=/opt/linux-device-build/packages/bin:/usr/local/venv/lib/python3.12/site-packages/gcc_arm_none_eabi/toolchain/bin:/usr/local/venv/lib/python3.12/site-packages/capnproto/install/bin:/usr/local/venv/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin \
    PYTHONPATH=/opt/linux-device-build/packages \
    CPATH=/usr/local/venv/lib/python3.12/site-packages/eigen/install \
    PYTHONUNBUFFERED=1 \
    PYTHONDONTWRITEBYTECODE=1 \
    UV_LINK_MODE=copy
SHELL ["/bin/bash", "-euo", "pipefail", "-c"]
# The optional secret supplies the Cloud proxy's public CA bundle locally.
# GitHub-hosted runners use normal rootfs CA trust and need no build secret.
RUN --mount=type=secret,id=proxy_ca \
    rm -f /TICI; \
    if [[ -f /run/secrets/proxy_ca ]]; then \
      export SSL_CERT_FILE=/run/secrets/proxy_ca PIP_CERT=/run/secrets/proxy_ca GIT_SSL_CAINFO=/run/secrets/proxy_ca; \
    fi; \
    /usr/local/venv/bin/python /opt/linux-device-build/bootstrap.py
WORKDIR /work
CMD ["/bin/bash"]
