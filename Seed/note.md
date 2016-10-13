# install library on ubuntu

1. install json-c

-	fix shared-library:
	+ use : -L directory -Wl,-rpath=directory
	+ sudo gedit /etc/ld.so.conf.d/local.conf
		add address of library
		sudo ldconfig
	+ or : export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:directory

# install library on pi

1. install json-c
-	sudo apt-get install autoconf
-	sudo apt-get install libtool-bin
-	sudo apt-get install libjson0-dbg


Chú ý libexample.so và libexample.a

Tên thư viện bao gồm : "lib + tên + đuôi mở rộng". ví dụ libexample.so thì tên thật là example được đính thêm "lib" đằng trước và đuôi là .so.
khi thư viện động libexample.so và thư viện tĩnh libexample.a có cùng tên và trong cùng một thư mục, compiler sẽ ưu tiên ghép thư viện động libexample.so vào trong chương trình chính và bỏ qua thư viện libexample.a. Khi đó muốn dùng thư viện tĩnh thì phải chỉ trực tiếp tới thư viện libexample.a hoặc sử dụng option -static của gcc/g++.

Nếu sử dụng libexample.so có vị trí khác vị trí mặc định của hệ thống thì cần thực hiện :

-	Cách 1 : Dùng option của gcc/g++ : -L "vị trí thư viện" -Wl,-rpath="vị trí thư viện"

-	Cách 2 : export LD_LIBRARY_PATH= "vị trí thư viện" 

-	Cách 3 : sudo gedit /etc/ld.so.conf.d/local.conf . Thêm "vị trí thư viện"

Để dùng với thư viện. Chương trình chính cần include file example.h. Khai báo thêm địa chỉ link thư viện -L"vị trí thư viện" và tên thư viện -lexample. Có thể cũng cần include vị trí file example.h vào.

Ví dụ vị trí thư viện libexample.so/ libexample.a là : /home/duong-exaple/lib .và vị trí của example.h là /home/duong-example/inlcude
ex : gcc -o main main.c -L /home/duong-exaple/lib -I /home/duong-exaple/include -lexample.
