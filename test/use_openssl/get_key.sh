#!/bin/bash
openssl genrsa -out localhost.key 2048 &&
echo "localhost.key was made." &&
openssl req -out localhost.csr -key localhost.key -new -nodes -subj "/C=JP/ST=Tokyo/L=Ropongi/CN=localhost"&&
echo "localhost.csr was made." &&
openssl x509 -req -days 1 -signkey localhost.key -in localhost.csr -out localhost.crt -extfile ${0%/*}/san.txt &&
echo "localhost.crt was made." 

#openssl req -x509 -newkey rsa:4096 -nodes -keyout localhost.key -out localhost.crt -days 1 -subj "/C=JP/ST=Tokyo/L=Ropongi/CN=Webserv.localhost" -extfile san.txt;
sudo security add-trusted-cert -d -r trustRoot -k "/Library/Keychains/System.keychain" "./localhost.crt" &&
echo "security was set."

# security add-trusted-cert -d -r trustRoot -k "/Library/Keychains/System.keychain" "証明書のパス"
#   証明書のインストール
# security
#   macOSにおいてセキュリティ関連の操作を行うためのコマンド。
# add-trusted-cert
#   certFileの証明書（DERまたはPEM形式）を、ユーザー単位またはローカル管理者の信頼設定に追加します。
# -d
#   admin cert storeに追加する。デフォルトはuser。
# -r trustRoot: 
#   登録する証明書に対して信頼レベルを設定します。trustRootは、システムの信頼されたルート認証局に証明書を追加することを意味します。
#   これにより、証明書の発行元が信頼された認証局である場合、それに基づいて署名されたすべての証明書を信頼することができます。
# -k /Library/Keychains/System.keychain: 
#   証明書を登録するキーチェーンの場所を指定します。この場合、/Library/Keychains/System.keychainに証明書が登録されます。