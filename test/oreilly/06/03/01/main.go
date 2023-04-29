package main

import (
	"fmt"
	"io"
	"log"
	"net/http"
	"net/http/httputil"
	"time"
)

func handlerUpgrade(w http.ResponseWriter, r *http.Request){

	if r.Header.Get("Connection") != "Upgrade" || r.Header.Get("Upgrade") != "MyProtocol" {
		w.WriteHeader(400)
		return
	}
	fmt.Println("Upgrade to MyProtocol")

	hijacker := w.(http.Hijacker)
	conn, readWriter, err := hijacker.Hijack()
	if err != nil{
		panic(err)
	}
	defer conn.Close()

	response := http.Response{
		StatusCode: 101,
		Header: make(http.Header),
	}
	response.Header.Set("Upgrade", "MyProtocol")
	response.Header.Set("Connection", "Upgrade")
	response.Write(conn)

	for i := 1; i <= 10; i++ {
		fmt.Fprintf(readWriter, "%d\n", i)
		fmt.Println("->", i)
		readWriter.Flush()
		recv, err := readWriter.ReadBytes('\n')
		if err == io.EOF{
			break
		}
		fmt.Printf("<-%s", string(recv))
		time.Sleep(500 * time.Millisecond)
	}
}

func handler(w http.ResponseWriter, r *http.Request){
	dump, err := httputil.DumpRequest(r, true)
	if err != nil {
		http.Error(w, fmt.Sprint(err), http.StatusInternalServerError)
	}
	fmt.Println(string(dump))
	fmt.Fprintf(w, "<html><body>hello</body></html>\n")
}

func main (){
	var httpServer http.Server
	http.HandleFunc("/", handler)
	http.HandleFunc("/upgrade", handlerUpgrade)
	log.Printf("start http listening :8080")
	httpServer.Addr = ":8080"
	println(httpServer.ListenAndServe())
}
