package main

import (
	"fmt"
	"time"

)

func main() {
	var pingpongCount int;
	ping := make(chan int)
	pong := make(chan int)
	done := make(chan struct{})
  	pingpongCount = 1000000

	startTime := time.Now()

	go func() {

		for n := 0; n < pingpongCount; n++ {
			ping <- n
			<-pong
		}

		close(ping)
		close(done)
	}()

	go func() {

		for n := range ping {
			pong <- n
		}

		close(pong)
	}()

	<-done
	
	endTime := time.Now()
	deltaT := endTime.Sub(startTime)
	time := float64(deltaT.Nanoseconds()) / 1000000000.0
	rate := float64(pingpongCount) / time
	fmt.Printf("Elapsed time: %v \t Number of messages: %v Number of Replies: \t%f \n", deltaT, pingpongCount, rate)
}
