package main

import(
	"log"
	"net/http"
	"net/url"
)

func main (){
	values := url.Values{
		"test":{"value"},
	}
	resp, err := http.PostForm("http://localhost:8080", values)
	if err != nil {
		panic(err)
	}
	log.Println("Status", resp.Status)
}