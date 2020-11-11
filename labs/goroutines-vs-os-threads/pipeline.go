package main

import (
	"flag"
	"fmt"
	"time"
	"strconv"
)

func main() {
	pipes := flag.Int("pipes", 1000000, "Number of pipes")
	verbose := flag.Bool("verbose", false, "Printing created pipes")
	flag.Parse()
	var start time.Time
	ch := make(chan struct{})
	in := ch
	start = time.Now()

	for i := 1; i <= *pipes; i++ {
		out := make(chan struct{})
		go func(in <-chan struct{}, out chan<- struct{}, i int) {
			out <- <-in
		}(in, out, i)

		in = out
		if *verbose {
			fmt.Printf("\r[%d] ", i)
		}
	}

	if *verbose {
		fmt.Println()
	}

	fmt.Printf("Goroutines created in %v\n", time.Since(start))
	start = time.Now()
	ch <- struct{}{}
	<-in
	fmt.Printf("Message transmitted in %v\n", time.Since(start))


  tme := time.Since(start)
  str := "Number of goroutines created: "+strconv.Itoa(*pipes)+"\nIt took "+tme.String() +" to transmit the message\n"
  fmt.Printf(str)

}
