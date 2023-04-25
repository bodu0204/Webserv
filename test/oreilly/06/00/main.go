package main

import(
	"io/ioutil"
	"log"
	"net/http"
)

func main (){
	resp, err := http.Get("https://localhost/")
	if err != nil {
		panic(err)
	}
	defer resp.Body.Close()

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		panic(err)
	}
	log.Println(resp.Header)
	log.Println(string(body))
}