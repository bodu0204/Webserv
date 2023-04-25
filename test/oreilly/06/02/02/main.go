package main 

import(
	"fmt"
	"log"
	"net/http"
	"net/http/httputil"
)

func handler(w http.ResponseWriter, r *http.Request){
	dump, err := httputil.DumpRequest(r, true)
	if err != nil {
		http.Error(w, fmt.Sprint(err), http.StatusInternalServerError)
	}
	fmt.Println(string(dump))
	fmt.Fprintf(w, "<html><body>hello</body></html>\n")
}

func main (){
	http.HandleFunc("/", handler)
	log.Printf("start http listening :8080")
	err := http.ListenAndServeTLS(":8080", "localhost.crt", "localhost.key", nil)
	log.Println(err)
}