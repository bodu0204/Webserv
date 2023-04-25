#!/bin/bash
# RSA 2048ビットの秘密鍵を作成
openssl genrsa -out ca.key 2048
# 証明書署名要求（CSR）を作成
openssl req -new -sha256 -key ca.key -out ca.csr -nodes #-config openssl.conf
# 証明書を自分の秘密鍵で署名して作成
openssl x509 -in ca.csr -days 365 -req signkey ca.key -sha256 -out ca,crt -extfile ./openssl.conf -extensions CA
