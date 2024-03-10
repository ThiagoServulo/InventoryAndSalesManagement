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

This tab is used to manage collaborators. After creating new collaborators, the table **(11)** will display a list of collaborators with their primary information. To view more details about a collaborator, select the corresponding row in the table **(11)**, and the fields "Name" **(1)**, "Telephone" **(2)**, and "Access type" **(3)** will then update to show the information for the selected collaborator.

If you wish to update the collaborator's information, first select the collaborator from table **(11)**, then modify the relevant field. modify the relevant field **(1, 2, or 3)** and then click on the "Save" button **(4)**. The same rules that apply to creating a collaborator must be followed. 

If you wish to remove a collaborator, first select the collaborator from table **(11)**, then click on the "Remove" button **(5)**.

To view the sales of a collaborator, select the collaborator from the table **(11)** and click on the "See sales" button **(6)**. A message box will display the sales IDs associated with this collaborator or a message indicating that the collaborator hasn't made any sales yet. For further details about a specific sale, please refer to the **"Sales Management window"**.

To reset a collaborator's password, first select the collaborator from the table **(11)**, then click on the "Reset Password" button **(7)**. This action will change the collaborator's password to the default password "1234". Upon their next login, the collaborator will be required to use this password and then set a new password.

To refine your selection of collaborators in table **(11)**, you may utilize the available filter options. First, select the filter type **(8)**, deciding whether to filter by the collaborator's ID or name. Then, enter the filter criterion **(9)** and click the "Filter" button **(10)**. Following this, only the collaborators matching the filter criteria will be displayed in table **(11)**. To reset the table and display all collaborators, simply leave the criterion field empty **(9)** and click the "Filter" button **(10)**.


### 4. Inventory management window

This window is designed to manage your inventory. It includes two tabs: one for adding new products and another for modifying existing product details, adjusting quantities, and updating prices.

#### 4.1. New product tab

![New product tab][new_product_tab]

This tab is dedicated to creating new products. To proceed, enter the product ID **(1)**, its description **(2)**, the purchase price **(3)**, the sale price **(4)**, the quantity in stock **(5)** and the supplier name **(6)**. 

It's important to inform that the sale price should be higher than the purchase price, and both should be greater than zero. The quantity should also be greater than zero, and the description and supplier should have more than 1 character. Finally, the ID should be different from zero, and the ID should not be registered in the database yet.

With the fields filled with the corrected values, to save the new product in the database, simply click on the "Save" button **(7)**. If you wish to cancel the operation, click on the "Cancel" button **(8)**.

#### 4.2. Inventory management tab

![Inventory management tab][inventory_management_tab]

This tab is used to manage the inventory. After creating new products, the table **(11)** will display a list of products with their primary information. To view more details about a products, select the corresponding row in the table **(11)**, and the fields "Description" **(1)**, "Quantity" **(2)**, "Supplier" **(3)**, "Purchase price" **(4)** and "Sale price" **(5)** will then update to show the information for the selected product.

If you wish to update the product's information, first select the product from table **(11)**, then modify the relevant field **(1, 2, 3, 4 ou 5)** and then click on the "Save" button **(6)**. The same rules that apply to creating a product must be followed. 

If you wish to remove a product, first select the product from table **(11)**, then click on the "Remove" button **(7)**.

To refine your selection of products in table **(11)**, you may utilize the available filter options. First, select the filter type **(8)**, deciding whether to filter by the prodcut's ID or description. Then, enter the filter criterion **(10)** and click the "Search" button **(9)**. Following this, only the products matching the filter criteria will be displayed in table **(11)**. To reset the table and display all products, simply leave the criterion field empty **(10)** and click the "Search" button **(9)**.


### 5. Sales management window

![Sales management window][sales_management_window]

This window is used to manage your sales. Table **(6)** will contain all the sales that your store has made, showing the sale ID, the date of the sale, the collaborator who made the sale, and the total value. If you select a sale in this table **(6)**, the other table **(7)** will display more information about the selected sale, such as the products included in the sale, the quantity of each product, the unit price, and the total price.

If you want to filter your sales, you can use the Initial and Final Date fields **(1)**, where you can define a period. After clicking the "Filter" button **(2)**, table **(6)** will display only the sales made within that period. To see all sales again, simply click on the "All Sales" button **(3)**.

To remove a sale, first select a sale in table **(6)**, then click on the "Remove" button **(4)**. If you want to export all your sales information, click on the "Export" button **(5)**, and a report will be generated in a text file containing the sales information


### 6. Sale window

![Sale window][sale_window]

This is the main window of the application, where you can create sales. First of all, it's important to note that to perform any action, such as creating a new sale or accessing other pages, it is necessary to log in using your username and password. To log in, simply click on the closed padlock icon **(10)** at the bottom of the window and enter your credentials. If the login is successful, your name will be displayed at the end of the window **(11)**, and the padlock icon will change to an open padlock **(10)**. To log off, just click on the open padlock **(10)**.

To access other pages, click on the action menu **(1)** at the top of the page. Under the "Management" option, you'll find access to the following windows: Collaborator Management, Inventory Management, and Sales Management. Within the "System" option, there are two actions: "Exit" to close the application, and "About" which will display some information about the system.

To create a sale, you must first log in. Next, you can specify the quantity of the product **(2)**. By default, the value of this field starts at 1. Also, enter the product ID **(3)**. This product ID must have been previously inserted into the database, and the required quantity should be available. Finally, click on the "Search Product" button **(4)** to add this product to your sale.

If the product is correctly added to the sale, it will be displayed in the table **(5)**. This table **(5)** will contain information about the current sale. The "Total" field **(6)** will show the total value of the sale, summing the values of each product included in the sale.

If you wish to remove a product from a sale, first select the product in the table **(5)**, then click on the "Remove product" button **(7)**.

If you wish to edit a product, such as changing the quantity or the sale price, simply select the product in the table **(5)** and click on the "Edit product" button **(8)**. After this, a new window will open where you can make the desired edits. This window will be discussed in the next topic.

After including all the products, you can finalize the sale by clicking the "Finalize sale" button **(9)**, and the new sale will be added to the database.


### 7. Edit product from a sale window

![Edit product from a sale window][edit_product_sale_window]

As mentioned earlier, to edit a product from a sale, you will use this window. Here, the fields ID product **(1)** and description **(2)** cannot be changed, so they are read-only. You can only modify the quantity **(3)** and sale price **(4)**. The new quantity should also be a valid value. After editing, click on the "Confirm" button **(5)** to confirm the changes or the "Cancel" button **(6)** to ignore the changes.

## How to use the program

To use the program you can download its source code and compile it using QT Creator, or simply run its executable. The "InventoryAndSalesManagement.exe" file, located in the executables folder, will run the application on your system and start a new database so that you have access to all the system's features. Always choose the most up-to-date version of the program available.

## Versions

### V1.0.1

* Date: 2024-03-10. 
* This is the fisrt version that contains all the functionalities describe in this document.

## Author

[@thiagoservulo](https://github.com/ThiagoServulo)


[login_window]: images/screenshots/login_window.png
[define_password_window]: images/screenshots/define_password_window.png
[new_collaborator_tab]: images/screenshots/new_collaborator.png
[collaborators_management_tab]: images/screenshots/collaborators_management.png
[new_product_tab]: images/screenshots/new_product.png
[inventory_management_tab]: images/screenshots/inventory_management.png
[sales_management_window]: images/screenshots/sales_management_window.png
[sale_window]: images/screenshots/sale_window.png
[edit_product_sale_window]: images/screenshots/edit_product_sale_window.png