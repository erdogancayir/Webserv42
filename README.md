# Webserv42
This project is here to make you write your own HTTP server. You will be able to test it with a real browser. HTTP is one of the most used protocol on internet. Knowing its arcane will be useful, even if you won't be working on a website. 
This is when you finally understand why a URL starts
with HTTP


### Contributors<br>
https://github.com/erdogancayir<br>
https://github.com/canodis<br>
https://github.com/hiqermod<br>



How to run ?

```  make ``` 

```  ./webserv config/ ``` 

<img width="1119" alt="Ekran Resmi 2023-02-14 ÖS 6 47 50" src="https://user-images.githubusercontent.com/94300378/218787732-3d25853c-ab1b-4231-9794-536e29fac50c.png">



![image](https://user-images.githubusercontent.com/94300378/215287887-7654f9b8-c402-4602-96f3-c3bc15b78a78.png)




```Features```

- user provided config file
- IO multiplexing: multiplexing of multiple sockets, using kqueue
- CGI/1.1 (dynamic content)
- HTTP/1.1 Protocol
- downloading a file, i.e. showing a static website (GET)
- uploading a file (POST)
- deleting a file (DELETE)

```Basics```

In this project we were challanged to create a webserver with quite a few functionalities that complies with the HTTP/1.1 standard. <br>
This, on its own is already a challenge when you started coding only 1.5 years ago.<br>
But to challange us even more we had to mainly use c++98 + and some c functions for alll the cases c++98 has no alternative like sockets and directory handling.<br>
First challenge was to have a config file that has a lot of functionalities and works kinda like a config file for nginx.<br>
The methods to implement where GET POST and DELETE.<br>
There had to be autoindexing as well as directory/file routing. Especially the directory/file routing is important to hide the actual directory structure of our server to the user that visites a website that is hosted on our server.<br>
One big and also complicated part was the implementation of chunked requests, because of the multiplexing.<br>
For our multiplexing we used kqueue, this enabled us to get the events (i.e. client connect, client send a request) directly from the kernel.<br>
For this reason our webserv is only running on macOS, since kqueue is not implemented in linux.<br>
The implementation of CGI's was a big part of the work too. On our webserv you can have as many cgi-extensions working as you like, as long as the cgi-executable is supplied.<br>
And last but not least we had to make sure our webserv never crashes and gives back appropriate error codes if something goes wrong.<br>

```Config File```

Theses following variables can be set inside the config file:

hostname

- mutliple ports
- turn on/off automatic directory listing
- setup of an index file (server-wide and directory specific)
- routing for
- certain directories
- certain file-extensions
- allowing methods for certain directories/file-extensions
- setup for the cgi
- setup of max_client_body_size (max number of bytes allowed in a single file/chunk)

Also you are able to setup multiple (virtual) servers in the same config file.
This will lead the webserver to be able to run multiple configs at the same time and applying them depending on which host was called in the request.



### Technologies

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Git](https://img.shields.io/badge/git-%23F05033.svg?style=for-the-badge&logo=git&logoColor=white) Git: Version control system <br>
![GitHub](https://img.shields.io/badge/github-%23121011.svg?style=for-the-badge&logo=github&logoColor=white) GitHub: Code repository and collaboration platform <br>
