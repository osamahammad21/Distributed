# What is this?
This is an Image sharing system based on a low level implmentation of UDP protocol that handles fragmentation and fault tollerance.


# Archticture
The project is Object Oriented through these classes:
- Message: class that represents the message entity that is used in message passing in the communication
- UDPSocket: the lowest level of the system that handles communcation on the UDP protocol level and that does fragmentation of messages. as a blackbox, it is a socket that sends a message passed to it and receives a message in the received messages queue.
- Peer: a higher level that represents the communicating entity which is the peer. it handles the RPC implmentation for the peer's operations.
- directoryServer: which represents another communicating entity that handles the directoryServer's operation.
- Image: represents the Image that is to be used on the system. it handles the steganogrophy of images, access management, formulation of all files related to the image.

# How to use
### UDPSocket
- initializing socket:
    ```c
    UDPSocket sock;
    sock.initializeSocket({PORT_NUMBER});
    ```
    replace *{PORT_NUMBER}* with an int representing the port number
-   sending message
    ```c
    Message *msg = new Message();
    //initialize msg and fill it
    if(sock.sendMessage(msg)){
        //message added to the send queue successfully
        cout<<"Yaay";
    }
    ```
-   receiving message
    ```c
    Message *msg = sock.receiveMsg();//blocks until a message is ready to be received
    ```
-   get socket info
    ```c
    string ip = sock.getMyIP();
    int port = sock.getMyPort();
    ```
### Image
for using this class you need to know which actor are you: 
- **creator** of a new image
- **owner** of an existing image(using it for editing)
- **viewer** of an image that is not owner by you

I am going to define code snippets for all test cases that may be required
- Creator
    ```c
    Image image;//create an object
    //you have to set the username and imageId in the beginning because it is needed for naming schemes
    image.setownerUsername("user1");//required in order
    image.setImageId("image123");//required in order
    //you supply the object with the path of the image
    image.chooseImage(string(IMAGE_DIR)+"small.jpg");//required in order
    struct userProperty prop;//this struct represent the setting for each user for the image
    prop.user_name="osama";
    prop.views=5;
    //access the properties of the image and edit as much as you want
    image.properties.push_back(prop);
    //write the properties to the properties file
    image.writeProperties();//required in order
    //steganize the image
    image.steg();//required in order
    //whenever you need to remove all other files except the steganized image file, call removeMiddleFiles
    image.removeMiddleFiles();
    ```
- Owner
    ```c
    Image image;
    //check if the image exists
    //findImage takes the username and imageId that are used for searching in the ImageDir
    if(!image.findImage("user1","image123"))
        //no Image Found
    
    ```
- viewer
    ```c
    string imagestr;//string that holds the image
    Image image;
    //check if the image exists
    //writeImage takes the string of the image and the owner(not the user's) username and imageId that are used for searching in the ImageDir
    image.writeImage(imagestr,"user1","image123");
    ```
-  Common Operations

    **1** read and edit properties

    ```c
    // readTheProperties and fill the propertiesVector
    image.readProperties();
    for(int i = 0;i<image.properties.size();i++)
    {
        if(image.properties[i].user_name=="osama"&&image.properties[i].views>0)
            image.properties[i].views+=10;
    }
    //updates the properties file but does not steganize
    image.updateProperties(); 
    ```
    **2** get Image Paths
    ```c
    string path1 = image.getAuthorizedImagePath();
    string path2 = image.getUnAuthorizedImagePath();
    ```
    **3** remove middle files
    ```c
    image.removeMiddleFiles();
    ```
    **4** steganize && desteganize
    ```c
    image.steg();
    image.desteg();
    ```
    **5** get stegImage in a string for sharing
    ```c
    string imageExtract = image.extractImage();
    ```
    **6** get the actual image's thumbnail in a string
    ```c
    image.desteg();//required in order
    string image64Extract = image.getSmallScaleImage();
    ```
    > it is base64_enoded twice so you need to decode it twice before writing to a file

### Peer

- Initialization
    ```c
    Peer peer(1);//port number
    peer.setDS("127.0.0.1",50);//the directoryService IP and port
    ```
- Powering the Status update thread
    ```c
    peer.startStatusUpdates("QisuAsSDAWeq126UhJS");//the token retreived after authentication
    ```
- Sample Functions
    ```c
    string token = peer.login("osama","123");//the username and password
    if(token=="not a user")
        cout<<"wrong username or password"<<endl;
    
    string token = peer.signup("osama","123");//the username and password
    if(token=="user already exists")
        cout<<"used username"<<endl;
    
    peer.addImageLocally("image123");//imageid
    peer.removeImageLocally("image123");

    string response = getAllImagesFromPeer("myUsername","theTargetPeerUsername","127.0.0.1",23);//the ip and port of the other peer
    if(response=="no photos")
        cout<<"no photos with that peer";
    else{
        vector<string> args;
        split(response,args,',');//you can copy the split function from the Peer.cpp class
        args[0];//the first encoded image received from the other peer
    }
    ```