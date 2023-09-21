#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <math.h>

// Structure to store latitude and longitude
typedef struct {
    double latitude;
    double longitude;
} Coordinates;

// Function to calculate the distance between two sets of coordinates
double calculateDistance(Coordinates coord1, Coordinates coord2) {
    // You can use a suitable formula to calculate the distance, e.g., Haversine formula
    // For simplicity, here's a basic Euclidean distance formula
    return sqrt(pow(coord1.latitude - coord2.latitude, 2) + pow(coord1.longitude - coord2.longitude, 2));
}

int main() {
    const char* xmlFilePath = "map.xml"; // Replace with your XML file path
    xmlDocPtr doc;
    xmlNodePtr node;

    // Load XML file
    doc = xmlReadFile(xmlFilePath, NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse document.\n");
        return 1;
    }

    // Prompt the user to enter the tag name
    char tagName[256];
    printf("Enter the tag name: ");
    scanf("%s", tagName);

    // Traverse the XML tree and process the desired tag
    node = xmlDocGetRootElement(doc);
    while (node != NULL) {
        if (xmlStrcmp(node->name, (const xmlChar*)tagName) == 0) {
            // Process this tag
            xmlNodePtr idNode = node->children;

            Coordinates prevCoord = {0.0, 0.0}; // Initialize with some default values
            while (idNode != NULL) {
                if (xmlStrcmp(idNode->name, (const xmlChar*)"id") == 0) {
                    // Extract latitude and longitude information
                    xmlChar* latStr = xmlGetProp(idNode, (const xmlChar*)"latitude");
                    xmlChar* lonStr = xmlGetProp(idNode, (const xmlChar*)"longitude");
                    if (latStr != NULL && lonStr != NULL) {
                        Coordinates currentCoord;
                        currentCoord.latitude = atof((const char*)latStr);
                        currentCoord.longitude = atof((const char*)lonStr);

                        // Calculate and print the distance to the previous coordinate
                        double distance = calculateDistance(prevCoord, currentCoord);
                        printf("Distance: %f\n", distance);

                        // Update the previous coordinate
                        prevCoord = currentCoord;
                    }
                }
                idNode = idNode->next;
            }
        }
        node = node->next;
    }

    // Cleanup
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return 0;
}

