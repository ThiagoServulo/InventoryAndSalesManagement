# Inventory and sales management

This project aims to develop an inventory and sales management system. It will be a straightforward system, allowing the creation and management of users with three different levels of access. Users will be able to create products, manage inventory (including quantities, prices, etc.), and oversee sales by adding, removing, and generating reports for visualization.

## Demonstration

### 1. Login window

![Login window][login_window]

This window serves as a simple login interface. Users are required to enter their username **(1)** and password **(2)** to access the program. After entering their credentials, they can proceed by clicking on the 'Login' button **(3)** or cancel the operation by clicking on 'Cancel' **(4)**. Additionally, there is a superuser account with the username 'admin' and password 'admin'. This superuser account grants unrestricted access to all program features, including the ability to create new accounts during the initial setup.


### 2. Define password window

![Define password window][define_password_window]

This is the 'Define Password' window. When a user is created, their password is set to '1234' by default. However, upon logging into the program for the first time, it is necessary to redefine the password. This window will appear to prompt the user to create a new password. The password must be entered in both the 'Password' **(1)** and 'Confirm Password' **(3)** fields, ensuring they match and are at least 6 characters in length. To reveal or conceal the password entered in each field, you can use buttons **2** and **4**. Once the password is created, you can click on the 'Confirm' button **(5)** to finalize the process.


### 3. Collaborator management window

This window is designed to manage your collaborators. It includes two tabs: one for creating a new collaborator, and the other for editing collaborator information, deleting collaborators, resetting passwords, or viewing sales.

#### 3.1. New collaborator tab

![New collaborator tab][new_collaborator_tab]

This tab is dedicated to creating new collaborators. To proceed, enter the collaborator's name **(1)**, an username for login purposes **(2)**, the telephone number **(3)**, and the type of access **(4)**. 

Both the name and the username must exceed 6 characters, while the telephone number should contain 11 digits. 

Regarding access types, there are three options: 

* **A** = Full access
* **B** = Creation privileges without editing capabilities
* **C** = Access limited to the sales page only.

After entering the collaborator's information, click on the "Save" button **(5)** to create the new collaborator, or "Cancel" **(6)** to abort the operation.

**Important:** When a new collaborator is created, their login will be performed using the created username and the initial password "1234". This password should be updated by the collaborator after their first login.

#### 3.2. Collaborators management tab

![Collaborators management tab][collaborators_management_tab]

This tab is used to manage collaborators. After creating new collaborators, the table **(11)** will display a list of collaborators with their primary information. To view more details about a collaborator, select the corresponding row in the table, and the fields "Name" **(1)**, "Telephone" **(2)**, and "Access type" **(3)** will then update to show the information for the selected collaborator.

If you wish to update the collaborator's information, first select the collaborator from table **(11)**, then modify the relevant field. modify the relevant field **(1, 2, or 3)** and then click on the "Save" button **(4)**. The same rules that apply to creating a collaborator must be followed. 

If you wish to remove a collaborator, first select the collaborator from table **(11)**, then click on the "Remove" button **(5)**.

To view the sales of a collaborator, select the collaborator from the table **(11)** and click on the "See sales" button **(6)**. A message box will display the sales IDs associated with this collaborator or a message indicating that the collaborator hasn't made any sales yet. For further details about a specific sale, please refer to the **"Sales Management window"**.

To reset a collaborator's password, first select the collaborator from the table **(11)**, then click on the "Reset Password" button **(7)**. This action will change the collaborator's password to the default password "1234". Upon their next login, the collaborator will be required to use this password and then set a new password.

[login_window]: images/screenshots/login_window.png
[define_password_window]: images/screenshots/define_password_window.png
[new_collaborator_tab]: images/screenshots/new_collaborator.png
[collaborators_management_tab]: images/screenshots/collaborators_management.png