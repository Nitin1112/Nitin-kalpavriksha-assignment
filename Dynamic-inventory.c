#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NAME_LENGTH 20

typedef struct Product
{
    int Id;
    char product_name[NAME_LENGTH];
    float price;
    int quantity;
} Product;

bool checkExistenceProduct(Product newProduct, Product *products, int size)
{
    for (int idx = 0; idx < size; idx++)
    {
        if (newProduct.Id == products[idx].Id)
        {
            return true;
        }
    }
    return false;
}

bool checkPriceRange(Product newProduct)
{
    if (newProduct.price <= 0)
        return true;
    return false;
}

bool checkInputtedValues(Product newProduct, Product *products, int size)
{
    if (checkExistenceProduct(newProduct, products, size))
    {
        printf("Product already exist\n");
        return false;
    }
    else if (checkPriceRange(newProduct))
    {
        printf("Invalid price range\n");
        return false;
    }
    return true;
}

Product getValues()
{
    Product temp_product;
    printf("Product ID: ");
    scanf("%d", &temp_product.Id);
    getchar();
    printf("Product Name: ");
    fgets(temp_product.product_name, NAME_LENGTH, stdin);
    temp_product.product_name[strcspn(temp_product.product_name, "\n")] = '\0';
    printf("Product Price: ");
    scanf("%f", &temp_product.price);
    printf("Product Quantity: ");
    scanf("%d", &temp_product.quantity);
    return temp_product;
}

Product *addNewProduct(Product *products, int *size)
{
    while (1)
    {
        printf("Enter new product details:\n");
        Product newProduct = getValues();
        if (!checkInputtedValues(newProduct, products, *size))
        {
            continue;
        }
        Product *newProducts = (Product *)realloc(products, (*size + 1) * sizeof(Product));
        if (newProducts == NULL)
        {
            printf("Failed to add new product\n");
            continue;
        }
        else
        {
            products = newProducts;
        }
        products[*size] = newProduct;
        (*size)++;
        printf("Product added successfully!\n");
        break;
    }
    return products;
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
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n", products[idx].Id, products[idx].product_name, products[idx].price, products[idx].quantity);
    }
}

void updateQuantity(Product *products, int size)
{
    int id;
    printf("Enter Product ID to update quantity: ");
    scanf("%d", &id);
    for (int i = 0; i < size; i++)
    {
        if (products[i].Id == id)
        {
            printf("Enter new quantity: ");
            scanf("%d", &products[i].quantity);
            printf("Quantity updated successfully!\n");
            return;
        }
    }
    printf("Product not found\n");
}

void searchById(Product *products, int size)
{
    int id;
    printf("Enter Product ID to search: ");
    scanf("%d", &id);
    for (int i = 0; i < size; i++)
    {
        if (products[i].Id == id)
        {
            printf("Product Found: Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",products[i].Id, products[i].product_name, products[i].price, products[i].quantity);
            return;
        }
    }
    printf("Product not found\n");
}

int containsSubstring(const char *str, const char *substr)
{
    int lenStr = strlen(str);
    int lenSub = strlen(substr);

    if (lenSub == 0 || lenSub > lenStr)
        return 0;

    for (int i = 0; i <= lenStr - lenSub; i++)
    {
        int j;
        for (j = 0; j < lenSub; j++)
        {
            if (str[i + j] != substr[j])
                break;
        }
        if (j == lenSub)
            return 1;
    }
    return 0;
}

void searchByName(Product *products, int size)
{
    char name[NAME_LENGTH];
    getchar();
    printf("Enter Name to search(partial allowed): ");
    fgets(name, NAME_LENGTH, stdin);
    name[strcspn(name, "\n")] = '\0';

    int found = 0;
    for (int i = 0; i < size; i++)
    {
        if (containsSubstring(products[i].product_name, name))
        {
            printf("Product Found: ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   products[i].Id, products[i].product_name,
                   products[i].price, products[i].quantity);
            found = 1;
        }
    }

    if (!found)
        printf("No matching products found.\n");
}

void searchByRange(Product *products, int size)
{
    float min, max;
    printf("Enter minimum price: ");
    scanf("%f", &min);
    printf("Enter maximum price: ");
    scanf("%f", &max);
    int found = 0;
    for (int i = 0; i < size; i++)
    {
        if (products[i].price >= min && products[i].price <= max)
        {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",products[i].Id, products[i].product_name, products[i].price, products[i].quantity);
            found = 1;
        }
    }
    if (!found)
        printf("No products in this price range\n");
}

void deleteProduct(Product **products, int *size)
{
    int id;
    printf("Enter Product ID to delete: ");
    scanf("%d", &id);
    int found = -1;
    for (int i = 0; i < *size; i++)
    {
        if ((*products)[i].Id == id)
        {
            found = i;
            break;
        }
    }
    if (found == -1)
    {
        printf("Product not found\n");
        return;
    }
    for (int i = found; i < *size - 1; i++)
    {
        (*products)[i] = (*products)[i + 1];
    }
    (*size)--;
    *products = (Product *)realloc(*products, (*size) * sizeof(Product));
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
            products = addNewProduct(products, size);
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
            searchByRange(products, *size);
            break;
        case 7:
            deleteProduct(&products, size);
            break;
        case 8:
            free(products);
            printf("memory released successfully. Exiting program...");
            return 0;
        default:
            printf("Enter valid option !!\n");
        }
    }
}

void dynamicInventory()
{
    int size = 0;
    printf("Enter the initial number of products: ");
    scanf("%d", &size);
    Product *products = (Product *)calloc(size, sizeof(Product));
    for (int idx = 0; idx < size; idx++)
    {
        printf("Enter details for product %d:\n", idx + 1);
        Product new_product = getValues();
        if (!checkInputtedValues(new_product, products, idx))
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
