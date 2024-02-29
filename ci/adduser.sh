#!/bin/bash
useradd -p "$(echo -n tdm_test | mkpasswd -m sha-512 -s)" -m tdm_test
