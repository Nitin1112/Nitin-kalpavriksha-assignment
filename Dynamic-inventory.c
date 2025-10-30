#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NAME_LENGTH 20

typedef struct Product
{
    int id;
    char name[NAME_LENGTH];
    float price;
    int quantity;
} Product;

bool isValidId(int id)
{
    if (id <= 0)
    {
        printf("Invalid id\n");
        return false;
    }
    return true;
}

int getStringLength(char *str)
{
    int idx = 0;
    while (str[idx] != '\0')
    {
        idx++;
    }
    return idx;
}

Product getValues()
{
    Product product;
    printf("Product ID: ");
    scanf("%d", &product.id);
    getchar();
    printf("Product Name: ");
    fgets(product.name, NAME_LENGTH, stdin);
    product.name[getStringLength(product.name) - 1] = '\0';
    printf("Product Price: ");
    scanf("%f", &product.price);
    printf("Product Quantity: ");
    scanf("%d", &product.quantity);
    return product;
}

bool isPresent(Product newProduct, Product *products, int size)
{
    for (int idx = 0; idx < size; idx++)
    {
        if (newProduct.id == products[idx].id)
        {
            printf("Product already exists\n");
            return true;
        }
    }
    return false;
}

bool isValidPriceRange(float price)  
{
    if (price <= 0)
    {
        printf("Invalid price range\n");
        return false;
    }
    return true;
}

bool isValidCharacter(char chr)  
{
    if ((chr >= 'a' && chr <= 'z') ||
        (chr >= 'A' && chr <= 'Z') ||
        (chr >= '0' && chr <= '9') ||
        chr == '-')
    {
        return true;
    }
    return false;
}

bool isValidProductName(char *str)
{
    for (int i = 0; i < getStringLength(str); i++)
    {
        if (!isValidCharacter(str[i]))
        {
            printf("Enter valid name\n");
            return false;
        }
    }
    return true;
}

bool isValidQuantity(int quantity)
{
    if (quantity < 0)
    {
        printf("Enter valid quantity\n");
        return false;
    }
    return true;
}

bool isValidValues(Product newProduct, Product *products, int size)
{
    if (!isValidId(newProduct.id))
    {
        return false;
    }
    else if (isPresent(newProduct, products, size))
    {
        return false;
    }
    else if (!isValidProductName(newProduct.name)) 
    {
        return false;
    }
    else if (!isValidPriceRange(newProduct.price))
    {
        return false;
    }
    else if (!isValidQuantity(newProduct.quantity))
    {
        return false;
    }
    return true;
}

void printProductDetails(Product **products, int *size, int idx)
{
    printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
           (*products)[idx].id, (*products)[idx].name,
           (*products)[idx].price, (*products)[idx].quantity);
}

void addNewProduct(Product **products, int *size)
{
    while (1)
    {
        printf("Enter new product details:\n");
        Product newProduct = getValues();
        if (!isValidValues(newProduct, *products, *size))
        {
            continue;
        }
        Product *newProducts = (Product *)realloc(*products, (*size + 1) * sizeof(Product));
        if (newProducts == NULL)
        {
            printf("Failed to add new product\n");
            continue;
        }
        else
        {
            *products = newProducts;
        }
        (*products)[*size] = newProduct;
        (*size)++;
        printf("Product added successfully!\n");
        break;
    }
}

void displayAllProducts(Product *products, int size)
{
    if (size == 0)
    {
        printf("No products available\n");
        return;
    }
    for (int idx = 0; idx < size; idx++)
    {
        printProductDetails(&products, &size, idx);
    }
}

void updateQuantity(Product *products, int size)
{
    int id;
    while (1)
    {
        printf("Enter Product ID to update quantity: ");
        scanf("%d", &id);
        if (!isValidId(id))
        {
            continue;
        }
        for (int idx = 0; idx < size; idx++)
        {
            if (products[idx].id == id)
            {
                int quantity = 0;
                printf("Enter new quantity: ");
                scanf("%d", &quantity);
                if (!isValidQuantity(quantity))
                {
                    continue;
                }
                else
                {
                    products[idx].quantity = quantity;
                }
                printf("Quantity updated successfully!\n");
                return;
            }
        }
        printf("Product not found\n");
        break;
    }
}

void searchById(Product *products, int size)
{
    int id;
    while (1)
    {
        printf("Enter Product ID to search: ");
        scanf("%d", &id);
        if (isValidId(id))
        {
            break;
        }
        else
        {
            printf("Invalid Id\n");
        }
    }

    for (int idx = 0; idx < size; idx++)
    {
        if (products[idx].id == id)
        {
            printf("Product Found: ");
            printProductDetails(&products, &size, idx);
            return;
        }
    }
    printf("Product not found\n");
}

int containsSubstring(char *str, char *substr)
{
    int lenStr = getStringLength(str);
    int lenSub = getStringLength(substr);

    if (lenSub == 0 || lenSub > lenStr)
        return 0;

    for (int skip = 0; skip <= lenStr - lenSub; skip++)
    {
        int idx;
        for (idx = 0; idx < lenSub; idx++)
        {
            if (str[skip + idx] != substr[idx])
                break;
        }
        if (idx == lenSub)
            return 1;
    }
    return 0;
}

void searchByName(Product *products, int size)
{
    char name[NAME_LENGTH];
    getchar();
    printf("Enter Name to search (partial allowed): ");
    fgets(name, NAME_LENGTH, stdin);
    name[getStringLength(name) - 1] = '\0';

    int found = 0;
    for (int idx = 0; idx < size; idx++)
    {
        if (containsSubstring(products[idx].name, name))
        {
            printf("Product Found: ");
            printProductDetails(&products, &size, idx);
            found = 1;
        }
    }

    if (!found)
        printf("No matching products found.\n");
}

void searchByPriceRange(Product *products, int size)
{
    float min, max;
    while (1)
    {
        printf("Enter minimum price: ");
        scanf("%f", &min);
        printf("Enter maximum price: ");
        scanf("%f", &max);
        if (min >= 0 && max >= 0 && min <= max)
        {
            break;
        }
        else
        {
            printf("Invalid values\n");
        }
    }
    int found = 0;
    for (int idx = 0; idx < size; idx++)
    {
        if (products[idx].price >= min && products[idx].price <= max)
        {
            printProductDetails(&products, &size, idx);
            found = 1;
        }
    }
    if (!found)
        printf("No products in this price range\n");
}

void deleteProduct(Product **products, int *size)
{
    int id;
    while (1)
    {
        printf("Enter Product ID to delete: ");
        scanf("%d", &id);
        if (isValidId(id))
        {
            break;
        }
        else
        {
            printf("Invalid Id\n");
        }
    }
    int found = -1;
    for (int idx = 0; idx < *size; idx++)
    {
        if ((*products)[idx].id == id)
        {
            found = idx;
            break;
        }
    }
    if (found == -1)
    {
        printf("Product not found\n");
        return;
    }
    for (int idx = found; idx < *size - 1; idx++)
    {
        (*products)[idx] = (*products)[idx + 1];
    }
    (*size)--;

    if (*size > 0)
        *products = (Product *)realloc(*products, (*size) * sizeof(Product));
    else
    {
        free(*products);
        *products = NULL;
    }

    printf("Product deleted successfully!\n");
}

int inventoryMenu(Product *products, int *size)
{
    int option = 0;
    while (1)
    {
        printf("\n========= INVENTORY MENU =========\n");
        printf("1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Update Quantity\n");
        printf("4. Search Product by ID\n");
        printf("5. Search Product by Name\n");
        printf("6. Search Product by Price Range\n");
        printf("7. Delete Product\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            addNewProduct(&products, size);
            break;
        case 2:
            displayAllProducts(products, *size);
            break;
        case 3:
            updateQuantity(products, *size);
            break;
        case 4:
            searchById(products, *size);
            break;
        case 5:
            searchByName(products, *size);
            break;
        case 6:
            searchByPriceRange(products, *size);
            break;
        case 7:
            deleteProduct(&products, size);
            break;
        case 8:
            free(products);
            printf("Memory released successfully. Exiting program...\n");
            return 0;
        default:
            printf("Enter valid option !!\n");
        }
    }
}

void dynamicInventory()
{
    int size = 0;
    while (1)
    {
        printf("Enter the initial number of products: ");
        scanf("%d", &size);
        if (size < 0)
        {
            printf("Invalid number\n");
        }
        else
            break;
    }
    Product *products = (Product *)calloc(size, sizeof(Product));
    for (int idx = 0; idx < size; idx++)
    {
        printf("Enter details for product %d:\n", idx + 1);
        Product new_product = getValues();
        if (!isValidValues(new_product, products, idx))
        {
            idx--;
            continue;
        }
        printf("\n");
        products[idx] = new_product;
    }
    inventoryMenu(products, &size);
}

int main()
{
    dynamicInventory();
    return 0;
}
