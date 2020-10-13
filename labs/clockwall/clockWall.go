package main

import (
	"fmt"
	"net"
	"os"
	"strings"
)

func main() {
	var ports []string
	for i := 1; i < len(os.Args); i++ {
		ports = append(ports, strings.Split(os.Args[i], ":")[1])
	}

	var connections []net.Conn
	var errors []error

	for _, port := range ports {
		conn, err := net.Dial("tcp", "localhost:"+port)
		if err == nil {
			connections = append(connections, conn)
			errors = append(errors, err)
		} else {
			fmt.Println("Could not connect to server with port " + port)
		}
	}

	for {

		for i := 0; i < len(connections); i++ {
			data := make([]byte, 512)
			n, err := connections[i].Read(data)
			if err != nil {
				panic(err)
			}
			fmt.Print(string(data[:n]))
		}
	}
}
