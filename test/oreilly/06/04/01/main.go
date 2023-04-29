package main

import (
	"fmt"
	"log"
	"net/http"
	"net/http/httputil"
	"time"
)

func handler(w http.ResponseWriter, r *http.Request){
	dump, err := httputil.DumpRequest(r, true)
	if err != nil {
		http.Error(w, fmt.Sprint(err), http.StatusInternalServerError)
	}
	fmt.Println(string(dump))
	fmt.Fprintf(w, "<html><body>hello</body></html>\n")
}

func handler_Chunked_Response(w http.ResponseWriter, r *http.Request){
	dump, err := httputil.DumpRequest(r, true)
	if err != nil {
		http.Error(w, fmt.Sprint(err), http.StatusInternalServerError)
	}
	fmt.Println(string(dump))
	flusher, ok := w.(http.Flusher)
	if !ok{
		panic("expected http.ResponseWriter to be an http.Fulusher")
	}
	for i := 1; i <= 10; i++ {
		fmt.Fprintf(w, "Chubk #%d\n", i)
		fmt.Printf(">Chubk #%d\n", i)
		flusher.Flush()
		time.Sleep(500 * time.Millisecond)
	}
	flusher.Flush()
}

func main (){
	var httpServer http.Server
	http.HandleFunc("/", handler)
	http.HandleFunc("/chunked", handler_Chunked_Response)
	log.Printf("start http listening :80")
	httpServer.Addr = ":80"
	println(httpServer.ListenAndServe())
}
