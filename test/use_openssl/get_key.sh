#!/bin/bash
openssl req -x509 -newkey rsa:4096 -nodes -keyout server.key -out server.crt -days 365 -subj "/C=JP/ST=Tokyo/L=Ropongi/O=42Tokyo/CN=blyu"

