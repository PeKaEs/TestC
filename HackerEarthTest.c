#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<float.h>

typedef struct{
  uint id;
  float averageTime;
  
} courierTimeID;

bool malloc2Dim( float *** gridArray, uint rowsN, uint colsM){
  uint i;

  *gridArray = malloc ( sizeof(float *) * rowsN);
  if (*gridArray == NULL )
      return false;

  for( i = 0; i < rowsN; ++i){
    (*gridArray)[i] = malloc( sizeof(float) * colsM);
    if((*gridArray)[i] == NULL)
      return false;
  }

  return true;
}

void countAverageTime( float courierRow[], uint courierSize, float citiesDistances[]){
  const float courierSpeed = courierRow[0] * 1.01666666667; //converting from km/h to km/min
  const float timePerPackage = courierRow[1];
  float numberOfPackages = 0;
  float totalTime = 0;
  uint i;

  for( i = 2 ; i<courierSize; ++i){
    if(courierRow[i] == 0)
      continue;

    numberOfPackages += courierRow[i];
    totalTime += courierRow[i] * timePerPackage;
    totalTime += citiesDistances[i - 2] / courierSpeed;
  }

  if(numberOfPackages != 0)
  courierRow[courierSize - 1] = totalTime / numberOfPackages;
  else
  courierRow[courierSize - 1] = FLT_MAX;
}

int comp(const void * elOne, const void * elTwo){

  courierTimeID first = *(( courierTimeID* ) elOne );
  courierTimeID second = *(( courierTimeID* ) elTwo );

  if(first.averageTime > second.averageTime) return 1;
  if(first.averageTime < second.averageTime) return -1;

  return 0;

}

int main()
{
    uint citiesCount = 0;
    uint couriersNum = 0;
    float *distances;
    float **couriers;
    courierTimeID *couriersMatrix;
    uint i = 0;
    uint j = 0;

    scanf("%u",&citiesCount);
    distances = malloc(sizeof(float)*citiesCount);
    if(distances == NULL)
      return 1;

    while (i < citiesCount && scanf("%f", &distances[i]) == 1)
         ++i;

    scanf("%u",&couriersNum);
    if(!malloc2Dim(&couriers, couriersNum, citiesCount+3))
      return 2;

    couriersMatrix = malloc(sizeof(courierTimeID) * couriersNum);
    if(couriersMatrix == NULL)
      return 2;

    for ( i = 0; i < couriersNum ; ++i){
      j = 0;
      while (j < citiesCount+2 && scanf("%f", &couriers[i][j]) == 1)
           ++j;
    }

    for( i = 0; i < couriersNum ; ++i){
      countAverageTime(couriers[i], citiesCount+3, distances);
    }

    for( i = 0; i < couriersNum; ++i){
      couriersMatrix[i].id = i;
      couriersMatrix[i].averageTime = couriers[i][citiesCount+2];
    }

    qsort(couriersMatrix, couriersNum, sizeof(courierTimeID), comp);

    for (i = 0 ; i < couriersNum; ++i){
      printf("%u ",couriersMatrix[i].id);
    }

    for ( i = 0 ; i < couriersNum; ++i){
      free(couriers[i]);
    }
    free(couriers);
    free(distances);
    free(couriersMatrix);

    return 0;
}
