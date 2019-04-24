/* 
 * File:   MCP48FEB22.h
 * Author: bmcgarvey
 *
 * Created on September 6, 2016, 11:33 AM
 */

#ifndef MCP48FEB22_H
#define	MCP48FEB22_H



#ifdef	__cplusplus
extern "C" {
#endif

void InitMCP48FEB22(void);
char MCP48FEB22WriteRegister(char reg, unsigned int value);
unsigned int MCP48FEB22ReadRegister(char reg);
void WriteDAC(char output, unsigned int value);


#ifdef	__cplusplus
}
#endif

#endif	/* MCP48FEB22_H */

