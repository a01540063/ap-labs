// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"os"
	"time"
	"strings"
)

//!+broadcaster
type clientCH chan<- string // an outgoing message channel

type client struct {
	channel clientCH
	name string
	ip string
	conn net.Conn
	isAdmin bool
}

var (
	clients  = make(map[clientCH]*client) // all connected clients
	entering = make(chan client)
	leaving  = make(chan clientCH)
	messages = make(chan string) // all incoming client messages
)

func broadcaster() {
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				cli <- msg
			}

		case clientInfo := <-entering:
			if len(clients) == 0 {
				clientInfo.isAdmin = true
			}
			clients[clientInfo.channel] = &clientInfo

		case cli := <-leaving:
			delete(clients, cli)
			close(cli)
		}
	}
}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn) {

	input := bufio.NewScanner(conn)
	input.Scan()
	who := input.Text()

	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)

	ip := conn.RemoteAddr().String()
	ch <- "irc-server > Welcome to the Simple IRC Server"
	ch <- "irc-server > Your user [" + who + "] is successfully logged"
	fmt.Println("irc-server > New connected user [" + who + "]")
	messages <- "irc-server > New connected user [" + who + "]"
	entering <- client{ch, who, ip, conn, false}
	
	// Admin
	if clients[ch].isAdmin == true{
		ch <- "irc-server > Congrats, you were the first user."
		fmt.Println("irc-server > [" + who + "] is now the ADMIN")
		ch <- "irc-server > You're the new IRC Server ADMIN"
	}

	for input.Scan() { // incoming client messages
		msg := input.Text()
		if msg != "" {
			if msg[0] == '/' {
				
				values := strings.Split(msg, " ")
				
				switch values[0] {
				case "/users":
					var userNames string
					for _,user := range clients{
						userNames += user.name + ", "
					}
					ch <- "irc-server > " + userNames[:len(userNames)-2]
				case "/msg":
					if len(values) < 3 {
						ch <- "irc-server > Error, usage: /msg <user> <msg>"
					} else {
						var found = false
						for _,user := range clients {
							if user.name == values[1] {
								found = true
								user.channel <- who + " (direct) > " + msg[6+len(user.name):]
								break;
							}
						}
						if !found {
							ch <- "irc-server > Error, user not found"
						}
					}
				case "/time":
					// Ingoring potential errors from time.LoadLocation("Local")
					location,_ := time.LoadLocation("Local")
					loc := location.String()
					// If TZ not specified load with America/Mexico_City
					if loc == "Local"{
						curr,_ := time.LoadLocation("America/Mexico_City")
						loc = curr.String()
					}
					ch <- "irc-server > Local Time: " + loc + " " + time.Now().Format("14:07")
				case "/user":
					if len(values) != 2 {
						ch <- "irc-server > Error, usage: /user <user>"
					} else {
						var found = false
						for _,user := range clients {
							if user.name == values[1] {
								found = true
								ch <- "irc-server > username: " + user.name + ", IP: " + user.ip
								break;
							}
						}
						if !found {
							ch <- "irc-server > Error, user not found"
						}
					}
				case "/kick":
					if clients[ch].isAdmin {
						if len(values) != 2 {
							ch <- "irc-server > Error, usage: /kick <user>"
						} else {
							var found = false
							for _,user := range clients {
								if user.name == values[1] {
									found = true
									user.channel <- "irc-server > " + "You're kicked from the channel"
									leaving <- user.channel
									fmt.Println("irc-server > " + user.name + " was kicked")
									messages <- "irc-server > " + user.name + " was kicked from the channel"
									user.conn.Close()
									break;
								}
							}
							if !found {
								ch <- "irc-server > User not found"
							}
						}
					} else {
						ch <- "irc-server > Error, you need to be ADMIN"
					}
				default:
					ch <- "irc-server > Error, avaliable commands: /users, /msg, /time, /user. /kick"
				}
			} else { // Message
				messages <- who + " > " + msg
			}
		}
	}
	if clients[ch] != nil {
		// NOTE: ignoring potential errors from input.Err()
		leaving <- ch
		fmt.Println("irc-server > [" + who + "] left")
		messages <- "irc-server > [" + who + "] left the channel"
		conn.Close()
	}
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

//!-handleConn

//!+main

func main() {
	if len(os.Args) != 5 {
		log.Fatal("Error, usage: go run client.go -host [host] -port [port]")
	}

	server := os.Args[2] + ":" + os.Args[4]
	listener, err := net.Listen("tcp", server)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println("irc-server > Simple IRC Server started at " + server)
	fmt.Println("irc-server > Ready for receiving new clients")

	go broadcaster()
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(conn)
	}
}

//!-main
