package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
)

//func handler(w http.ResponseWriter, r *http.Request)  {
//	pusher, ok := w.(http.Pusher)
//	if ok {
//		pusher.Push("/style.css", nil)
//	}
//	//通常の処理
//}


var image []byte

func init(){
	var err error
	image, err = ioutil.ReadFile("./image.jpeg")
	if err != nil{
		panic(err)
	}
}

func handlerHtml(w http.ResponseWriter, r *http.Request){
	pusher, ok := w.(http.Pusher)
	if ok{
		err :=pusher.Push("/image", nil)
		if err != nil {
			fmt.Println("Push error:", err)
		}
	}
	w.Header().Add("Content-Type", "text/html")
	fmt.Fprintf(w, `<html><body><img src="/image"></body></html>`)
}

func handlerImage(w http.ResponseWriter, r *http.Request){
	w.Header().Set("Content-type", "image/jpeg")
	w.Write(image)
}

func main(){
	http.HandleFunc("/", handlerHtml)
	http.HandleFunc("/image", handlerImage)
	fmt.Println("start http lisning :443")
	err := http.ListenAndServeTLS(":443", "localhost.crt", "localhost.key", nil)
	if err != nil {
		panic(err)
	}
}