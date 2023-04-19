package main

import(
	"log"
	"net/http"
)

func main (){
	resp, err := http.Head("http://localhost:8080")
	if err != nil {
		panic(err)
	}
	log.Println("Status", resp.Status)
	log.Println("Header", resp.Header)
}