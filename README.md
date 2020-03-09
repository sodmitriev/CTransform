# CTransform
A minimalistic ETL implementation for C with multiple subsequent transformations.

This project implements a simple interface for manipulations over a data stream. For example, is may be used to 
painlessly read data from a compressed or encrypted file (or both) without explicitly calling any function of a 
compression or encryption library.

A uniform interfaces for transformations, data source and sink are provided, so you may implement your own operations
over the data stream and add it into the process without rewriting any of this project's source code.

This code is written on "C" language, but it heavily exploits concepts of inheritance and exceptions (via  
CEasyException library). Use examples to get used with the way in which inheritance is implemented in this project.

## Links
- [source code documentation](https://sodmitriev.github.io/CTransform)
- [usage examples](https://sodmitriev.github.io/CTransform/dir_d28a4824dc47e487b107a5db32ef43c4.html)
- [controller](https://sodmitriev.github.io/CTransform/structcontroller.html): transformation 
process executor
- [source](https://sodmitriev.github.io/CTransform/structsource.html): interface for data source classes
- [sink](https://sodmitriev.github.io/CTransform/structsink.html): interface for data sink classes
- [transformation](https://sodmitriev.github.io/CTransform/structtransformation.html): interface for data transformation
 classes