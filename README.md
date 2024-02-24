# Inventory and sales management

This project aims to develop an inventory and sales management system. It will be a straightforward system, allowing the creation and management of users with three different levels of access. Users will be able to create products, manage inventory (including quantities, prices, etc.), and oversee sales by adding, removing, and generating reports for visualization.

## Demonstration

### 1. Login window

![Login window][login_window]

This window serves as a simple login interface. Users are required to enter their username **(1)** and password **(2)** to access the program. After entering their credentials, they can proceed by clicking on the 'Login' button **(3)** or cancel the operation by clicking on 'Cancel' **(4)**. Additionally, there is a superuser account with the username 'admin' and password 'admin'. This superuser account grants unrestricted access to all program features, including the ability to create new accounts during the initial setup.


### 2. Define password window

![Define password window][define_password_window]

This is the 'Define Password' window. When a user is created, their password is set to '1234' by default. However, upon logging into the program for the first time, it is necessary to redefine the password. This window will appear to prompt the user to create a new password. The password must be entered in both the 'Password' **(1)** and 'Confirm Password' **(3)** fields, ensuring they match and are at least 6 characters in length. To reveal or conceal the password entered in each field, you can use buttons **2** and **4**. Once the password is created, you can click on the 'Confirm' button **(5)** to finalize the process.


[login_window]: images/screenshots/login_window.png
[define_password_window]: images/screenshots/define_password_window.png
