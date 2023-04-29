package main

import(
	"bufio"
	"bytes"
	"fmt"
	"io"
	"log"
	"net"
	"net/http"
	"time"
)

func main(){
	dialer := &net.Dialer{
		Timeout: 30 * time.Second,
		KeepAlive: 30 * time.Second,
	}
	conn, err := dialer.Dial("tcp", "localhost:8080")
	if err != nil{
		panic(err)
	}
	defer conn.Close()
	reader := bufio.NewReader(conn)

	request, _ := http.NewRequest("GET", "http://localhost:8080/upgrade", nil)
	request.Header.Set("Connection", "Upgrade")
	request.Header.Set("Upgrade", "MyProtocol")
	err = request.Write(conn)
	if err != nil{
		panic(err)
	}

	resp, err := http.ReadResponse(reader, request)
	if err != nil{
		panic(err)
	}
	var buf bytes.Buffer
	io.Copy(&buf, resp.Body)

	log.Println("Status:", resp.Status)
	log.Println("Headers:", resp.Header)
	log.Println("Body:", buf.String())

	counter := 10
	for {
		data, err := reader.ReadBytes('\n')
		if err == io.EOF{
			break
		}
		fmt.Println("<-", string(bytes.TrimSpace(data)))
		fmt.Fprintf(conn, "%d\n", counter)
		fmt.Println("->", counter)
		counter--
	}
}
