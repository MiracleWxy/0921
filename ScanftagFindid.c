#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

// 定义一个函数来处理XML节点
void processNode(xmlNode *node, const char *searchKey, const char *searchValue) {
    xmlNode *currentNode = NULL;
    for (currentNode = node; currentNode; currentNode = currentNode->next) {
        if (currentNode->type == XML_ELEMENT_NODE) {
            if (strcmp((char *)currentNode->name, "tag") == 0) {
                // 查找tag节点
                xmlChar *k = xmlGetProp(currentNode, (xmlChar *)"k");
                xmlChar *v = xmlGetProp(currentNode, (xmlChar *)"v");
                if (k && v && strcmp((char *)k, searchKey) == 0 && strcmp((char *)v, searchValue) == 0) {
                    // 找到符合条件的节点，打印其父节点的id属性值
                    xmlNode *parentNode = currentNode->parent;
                    xmlChar *parentId = xmlGetProp(parentNode, (xmlChar *)"id");
                    if (parentId) {
                        printf("Found matching node with id: %s\n", parentId);
                        xmlFree(parentId);
                    }
                }
                if (k) xmlFree(k);
                if (v) xmlFree(v);
            }
            // 递归处理子节点
            processNode(currentNode->children, searchKey, searchValue);
        }
    }
}

int main() {
    char searchKey[50];
    char searchValue[50];

    printf("Enter the tag k value: ");
    scanf("%s", searchKey);
    printf("Enter the tag v value: ");
    scanf("%s", searchValue);

    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    // 解析XML文件
    doc = xmlReadFile("map.osm", NULL, 0);
    if (doc == NULL) {
        printf("Error loading XML file.\n");
        return 1;
    }

    root_element = xmlDocGetRootElement(doc);

    // 处理根节点
    processNode(root_element, searchKey, searchValue);

    // 释放资源
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}

