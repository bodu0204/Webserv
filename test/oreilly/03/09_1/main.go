package main

import (
	"bytes"
	"io"
	"log"
	"mime/multipart"
	"net/http"
	"net/textproto"
	"os"
	//"path/filepath"
)

func main (){
	var buffer bytes.Buffer

	writer := multipart.NewWriter(&buffer)
	writer.WriteField("name", "M_J")
	part := make(textproto.MIMEHeader)
	part.Set("Content-Type", "image/jpeg")
	part.Set("Content-Disposition", `form-data; name="thumdnail"; filename="photo.jpg"`)
	fileWriter, err := writer.CreatePart(part)
	readFile, err := os.Open("photo.jpg")
	if err != nil {
		panic(err)
	}
	io.Copy(fileWriter, readFile)


	resp, err := http.Post("http://localhost:8080", writer.FormDataContentType(), &buffer)
	if err != nil {
		panic(err)
	}
	log.Println("Status", resp.Status)
}