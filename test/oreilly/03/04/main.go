package main

import(
	"io/ioutil"
	"log"
	"net/http"
)

func main (){
	resp, err := http.Get("http://localhost:8080")
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