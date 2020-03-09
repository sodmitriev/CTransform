# CTransform
A minimalistic ETL implementation for C with multiple subsequent transformations

This project implements a simple interface for manipulations over a data stream. For example, is may be used to 
painlessly read data from a compressed or encrypted file (or both) without explicitly calling any function of a 
compression or encryption library.

A uniform interfaces for transformations, data source and sink are provided, so you may implement your own operations
over the data stream and add it into the process without rewriting any of this project's source code.

This code is written on "C" language, but it heavily exploits concepts of inheritance and exceptions (via  
CEasyException library). Use examples to get used with the way in which inheritance is implemented in this project.

A set of [examples](https://sodmitriev.github.io/CTransform/dir_d28a4824dc47e487b107a5db32ef43c4.html) is written to 
demonstrate how this library may be used. To start learning about the concept implemented by this library, refer to it's 
main "class" [controller](https://sodmitriev.github.io/CTransform/structcontroller.html), or one of 3 "interface" 
classes: [source](https://sodmitriev.github.io/CTransform/structsource.html), 
[sink](https://sodmitriev.github.io/CTransform/structsink.html) and 
[transformation](https://sodmitriev.github.io/CTransform/structtransformation.html).