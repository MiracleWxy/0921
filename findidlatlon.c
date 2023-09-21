#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

int main() {
    xmlDocPtr doc;
    xmlNodePtr rootNode, currentNode, ndNode;
    
    // 1. 初始化libxml2库
    LIBXML_TEST_VERSION;

    // 2. 打开XML文件并解析
    doc = xmlReadFile("map.xml", NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse the XML file.\n");
        return 1;
    }

    // 3. 初始化数组和计数器
    unsigned int idArray[12000]; // 假设最多有100个匹配的节点
    double latArray[12000];
    double lonArray[12000];
    int idCount = 0;

    // 4. 遍历XML文档的节点
    rootNode = xmlDocGetRootElement(doc);
    currentNode = rootNode->xmlChildrenNode;

    while (currentNode != NULL) {
        if (xmlStrcmp(currentNode->name, (const xmlChar *)"way") == 0) {
            // 在名称为way的节点下查找名称为nd的节点
            ndNode = currentNode->xmlChildrenNode;
            while (ndNode != NULL) {
                if (xmlStrcmp(ndNode->name, (const xmlChar *)"nd") == 0) {
                    xmlChar *refValue = xmlGetProp(ndNode, (const xmlChar *)"ref");
                    if (refValue != NULL) {
                        // 在osm节点下查找匹配的名称为node的子节点
                        xmlNodePtr nodeNode = rootNode->xmlChildrenNode;
                        while (nodeNode != NULL) {
                            if (xmlStrcmp(nodeNode->name, (const xmlChar *)"node") == 0) {
                                xmlChar *idValue = xmlGetProp(nodeNode, (const xmlChar *)"id");
                                xmlChar *latValue = xmlGetProp(nodeNode, (const xmlChar *)"lat");
                                xmlChar *lonValue = xmlGetProp(nodeNode, (const xmlChar *)"lon");
                                
                                if (idValue != NULL && latValue != NULL && lonValue != NULL) {
                                    if (xmlStrcmp(refValue, idValue) == 0) {
                                        // 将id、lat和lon存入相应的数组中
                                        sscanf((char *)idValue, "%u", &idArray[idCount]);
                                        sscanf((char *)latValue, "%lf", &latArray[idCount]);
                                        sscanf((char *)lonValue, "%lf", &lonArray[idCount]);
                                        idCount++;
                                    }
                                }

                                xmlFree(idValue);
                                xmlFree(latValue);
                                xmlFree(lonValue);
                            }
                            nodeNode = nodeNode->next;
                        }
                    }
                    xmlFree(refValue);
                }
                ndNode = ndNode->next;
            }
        }
        currentNode = currentNode->next;
    }

    // 5. 打印存储的数据
   // printf("Matching Nodes:\n");
   // for (int i = 0; i < idCount; i++) {
    //    printf("Node ID: %u, Latitude: %lf, Longitude: %lf\n", idArray[i], latArray[i], lonArray[i]);
    //}

    // 6. 清理资源
    xmlFreeDoc(doc);
    xmlCleanupParser();
    
    return 0;
}

