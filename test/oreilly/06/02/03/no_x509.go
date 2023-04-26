package main

import (
	"crypto/tls"
	"crypto/x509"
	"io/ioutil"
	"log"
	"net/http"
	"net/httputil"
)

func main(){
	cert, err := ioutil.ReadFile("ca.crt")
	if err != nil{
		panic(err)
	}
	certPool := x509.NewCertPool()
	certPool.AppendCertsFromPEM(cert)
	tlsConfig := &tls.Config{
		RootCAs: certPool,
	}
	tlsConfig.BuildNameToCertificate()
	client := &http.Client{
		Transport: &http.Transport{
			TLSClientConfig: tls,
		},
	}
}
