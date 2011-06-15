/* ============================================================

Copyright (c) 2009 Advanced Micro Devices, Inc.  All rights reserved.

Redistribution and use of this material is permitted under the following 
conditions:

Redistributions must retain the above copyright notice and all terms of this 
license.

In no event shall anyone redistributing or accessing or using this material 
commence or participate in any arbitration or legal action relating to this 
material against Advanced Micro Devices, Inc. or any copyright holders or 
contributors. The foregoing shall survive any expiration or termination of 
this license or any agreement or access or use related to this material. 

ANY BREACH OF ANY TERM OF THIS LICENSE SHALL RESULT IN THE IMMEDIATE REVOCATION 
OF ALL RIGHTS TO REDISTRIBUTE, ACCESS OR USE THIS MATERIAL.

THIS MATERIAL IS PROVIDED BY ADVANCED MICRO DEVICES, INC. AND ANY COPYRIGHT 
HOLDERS AND CONTRIBUTORS "AS IS" IN ITS CURRENT CONDITION AND WITHOUT ANY 
REPRESENTATIONS, GUARANTEE, OR WARRANTY OF ANY KIND OR IN ANY WAY RELATED TO 
SUPPORT, INDEMNITY, ERROR FREE OR UNINTERRUPTED OPERA TION, OR THAT IT IS FREE 
FROM DEFECTS OR VIRUSES.  ALL OBLIGATIONS ARE HEREBY DISCLAIMED - WHETHER 
EXPRESS, IMPLIED, OR STATUTORY - INCLUDING, BUT NOT LIMITED TO, ANY IMPLIED 
WARRANTIES OF TITLE, MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
ACCURACY, COMPLETENESS, OPERABILITY, QUALITY OF SERVICE, OR NON-INFRINGEMENT. 
IN NO EVENT SHALL ADVANCED MICRO DEVICES, INC. OR ANY COPYRIGHT HOLDERS OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, REVENUE, DATA, OR PROFITS; OR 
BUSINESS INTERRUPTION) HOWEVER CAUSED OR BASED ON ANY THEORY OF LIABILITY 
ARISING IN ANY WAY RELATED TO THIS MATERIAL, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE. THE ENTIRE AND AGGREGATE LIABILITY OF ADVANCED MICRO DEVICES, 
INC. AND ANY COPYRIGHT HOLDERS AND CONTRIBUTORS SHALL NOT EXCEED TEN DOLLARS 
(US $10.00). ANYONE REDISTRIBUTING OR ACCESSING OR USING THIS MATERIAL ACCEPTS 
THIS ALLOCATION OF RISK AND AGREES TO RELEASE ADVANCED MICRO DEVICES, INC. AND 
ANY COPYRIGHT HOLDERS AND CONTRIBUTORS FROM ANY AND ALL LIABILITIES, 
OBLIGATIONS, CLAIMS, OR DEMANDS IN EXCESS OF TEN DOLLARS (US $10.00). THE 
FOREGOING ARE ESSENTIAL TERMS OF THIS LICENSE AND, IF ANY OF THESE TERMS ARE 
CONSTRUED AS UNENFORCEABLE, FAIL IN ESSENTIAL PURPOSE, OR BECOME VOID OR 
DETRIMENTAL TO ADVANCED MICRO DEVICES, INC. OR ANY COPYRIGHT HOLDERS OR 
CONTRIBUTORS FOR ANY REASON, THEN ALL RIGHTS TO REDISTRIBUTE, ACCESS OR USE 
THIS MATERIAL SHALL TERMINATE IMMEDIATELY. MOREOVER, THE FOREGOING SHALL 
SURVIVE ANY EXPIRATION OR TERMINATION OF THIS LICENSE OR ANY AGREEMENT OR 
ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE IS HEREBY PROVIDED, AND BY REDISTRIBUTING OR ACCESSING OR USING THIS 
MATERIAL SUCH NOTICE IS ACKNOWLEDGED, THAT THIS MATERIAL MAY BE SUBJECT TO 
RESTRICTIONS UNDER THE LAWS AND REGULATIONS OF THE UNITED STATES OR OTHER 
COUNTRIES, WHICH INCLUDE BUT ARE NOT LIMITED TO, U.S. EXPORT CONTROL LAWS SUCH 
AS THE EXPORT ADMINISTRATION REGULATIONS AND NATIONAL SECURITY CONTROLS AS 
DEFINED THEREUNDER, AS WELL AS STATE DEPARTMENT CONTROLS UNDER THE U.S. 
MUNITIONS LIST. THIS MATERIAL MAY NOT BE USED, RELEASED, TRANSFERRED, IMPORTED,
EXPORTED AND/OR RE-EXPORTED IN ANY MANNER PROHIBITED UNDER ANY APPLICABLE LAWS, 
INCLUDING U.S. EXPORT CONTROL LAWS REGARDING SPECIFICALLY DESIGNATED PERSONS, 
COUNTRIES AND NATIONALS OF COUNTRIES SUBJECT TO NATIONAL SECURITY CONTROLS. 
MOREOVER, THE FOREGOING SHALL SURVIVE ANY EXPIRATION OR TERMINATION OF ANY 
LICENSE OR AGREEMENT OR ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE REGARDING THE U.S. GOVERNMENT AND DOD AGENCIES: This material is 
provided with "RESTRICTED RIGHTS" and/or "LIMITED RIGHTS" as applicable to 
computer software and technical data, respectively. Use, duplication, 
distribution or disclosure by the U.S. Government and/or DOD agencies is 
subject to the full extent of restrictions in all applicable regulations, 
including those found at FAR52.227 and DFARS252.227 et seq. and any successor 
regulations thereof. Use of this material by the U.S. Government and/or DOD 
agencies is acknowledgment of the proprietary rights of any copyright holders 
and contributors, including those of Advanced Micro Devices, Inc., as well as 
the provisions of FAR52.227-14 through 23 regarding privately developed and/or 
commercial computer software.

This license forms the entire agreement regarding the subject matter hereof and 
supersedes all proposals and prior discussions and writings between the parties 
with respect thereto. This license does not affect any ownership, rights, title,
or interest in, or relating to, this material. No terms of this license can be 
modified or waived, and no breach of this license can be excused, unless done 
so in a writing signed by all affected parties. Each term of this license is 
separately enforceable. If any term of this license is determined to be or 
becomes unenforceable or illegal, such term shall be reformed to the minimum 
extent necessary in order for this license to remain in effect in accordance 
with its terms as modified by such reformation. This license shall be governed 
by and construed in accordance with the laws of the State of Texas without 
regard to rules on conflicts of law of any state or jurisdiction or the United 
Nations Convention on the International Sale of Goods. All disputes arising out 
of this license shall be subject to the jurisdiction of the federal and state 
courts in Austin, Texas, and all defenses are hereby waived concerning personal 
jurisdiction and venue of these courts.

============================================================ */


#include "NBodySimple.hpp"
#include<GL/glut.h>
#include <cmath>
#include<malloc.h>

int numBodies;      /**< No. of particles*/
cl_float* posX;      /**< Output position */
cl_float* posY;      /**< Output position */
cl_float* posZ;      /**< Output position */
cl_float* posW;      /**< Output position */
void* me;           /**< Pointing to NBodySimple class */
cl_bool display;

float
NBodySimple::random(float randMax, float randMin)
{
    float result;
    result =(float)rand()/(float)RAND_MAX;

    return ((1.0f - result) * randMin + result *randMax);
}

int
NBodySimple::setupNBodySimple()
{
    // make sure numParticles is multiple of group size
    numParticles = (cl_int)(((size_t)numParticles 
        < groupSize) ? groupSize : numParticles);
    numParticles = (cl_int)((numParticles / groupSize) * groupSize);

    numBodies = numParticles;

    initPosX = (cl_float*)malloc(numBodies * sizeof(cl_float));
    initPosY = (cl_float*)malloc(numBodies * sizeof(cl_float));
    initPosZ = (cl_float*)malloc(numBodies * sizeof(cl_float));
    initPosW = (cl_float*)malloc(numBodies * sizeof(cl_float));
    if(initPosX == NULL || initPosY == NULL || initPosZ == NULL || initPosW == NULL)	
    { 
        sampleCommon->error("Failed to allocate host memory. (initPos)");
        return SDK_FAILURE;
    }

    initVelX = (cl_float*)malloc(numBodies * sizeof(cl_float));
    initVelY = (cl_float*)malloc(numBodies * sizeof(cl_float));
    initVelZ = (cl_float*)malloc(numBodies * sizeof(cl_float));
    if(initVelX == NULL || initVelY == NULL || initVelZ == NULL)	
    { 
        sampleCommon->error("Failed to allocate host memory. (initVel)");
        return SDK_FAILURE;
    }

#if defined (_WIN32)
    posX = (cl_float*)_aligned_malloc(numBodies * sizeof(cl_float), 16);
    posY = (cl_float*)_aligned_malloc(numBodies * sizeof(cl_float), 16);
    posZ = (cl_float*)_aligned_malloc(numBodies * sizeof(cl_float), 16);
    posW = (cl_float*)_aligned_malloc(numBodies * sizeof(cl_float), 16);
#else
    posX = (cl_float*)memalign(16, numBodies * sizeof(cl_float));
    posY = (cl_float*)memalign(16, numBodies * sizeof(cl_float));
    posZ = (cl_float*)memalign(16, numBodies * sizeof(cl_float));
    posW = (cl_float*)memalign(16, numBodies * sizeof(cl_float));
#endif
    if(posX == NULL || posY == NULL || posZ == NULL || posW == NULL)
    { 
        sampleCommon->error("Failed to allocate host memory. (pos)");
        return SDK_FAILURE;
    }

#if defined (_WIN32)
    velX = (cl_float*)_aligned_malloc(numBodies * sizeof(cl_float), 16);
    velY = (cl_float*)_aligned_malloc(numBodies * sizeof(cl_float), 16);
    velZ = (cl_float*)_aligned_malloc(numBodies * sizeof(cl_float), 16);
#else
    velX = (cl_float*)memalign(16, numBodies * sizeof(cl_float));
    velY = (cl_float*)memalign(16, numBodies * sizeof(cl_float));
    velZ = (cl_float*)memalign(16, numBodies * sizeof(cl_float));
#endif

    if(velX == NULL || velY == NULL || velZ == NULL)
    { 
        sampleCommon->error("Failed to allocate host memory. (vel)");
        return SDK_FAILURE;
    }

    /* initialization of inputs */
    for(int i = 0; i < numBodies; ++i)
    {
        int index = i;

        // First 3 values are position in x,y and z direction
		initPosX[index] = random(1, 50);
		initPosY[index] = random(1, 50);
		initPosZ[index] = random(1, 50);

        // Mass value
        initPosW[index] = random(1, 1000);

        // First 3 values are velocity in x,y and z direction
		initVelX[index] = 0.0f;
		initVelY[index] = 0.0f;
		initVelZ[index] = 0.0f;
    }

    memcpy(posX, initPosX, numBodies * sizeof(cl_float));
    memcpy(posY, initPosY, numBodies * sizeof(cl_float));
    memcpy(posZ, initPosZ, numBodies * sizeof(cl_float));
    memcpy(posW, initPosW, numBodies * sizeof(cl_float));
    memcpy(velX, initVelX, numBodies * sizeof(cl_float));
    memcpy(velY, initVelY, numBodies * sizeof(cl_float));
    memcpy(velZ, initVelZ, numBodies * sizeof(cl_float));

    return SDK_SUCCESS;
}

int
NBodySimple::setupCL()
{
    cl_int status = CL_SUCCESS;

    cl_device_type dType;

    if(deviceType.compare("cpu") == 0)
    {
        dType = CL_DEVICE_TYPE_CPU;
    }
    else //deviceType = "gpu" 
    {
        dType = CL_DEVICE_TYPE_GPU;
    }

    /*
     * Have a look at the available platforms and pick either
     * the AMD one if available or a reasonable default.
     */

    cl_uint numPlatforms;
    cl_platform_id platform = NULL;
    status = clGetPlatformIDs(0, NULL, &numPlatforms);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clGetPlatformIDs failed."))
    {
        return SDK_FAILURE;
    }
    if (0 < numPlatforms) 
    {
        cl_platform_id* platforms = new cl_platform_id[numPlatforms];
        status = clGetPlatformIDs(numPlatforms, platforms, NULL);
        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clGetPlatformIDs failed."))
        {
            return SDK_FAILURE;
        }
        for (unsigned i = 0; i < numPlatforms; ++i) 
        {
            char pbuf[100];
            status = clGetPlatformInfo(platforms[i],
                                       CL_PLATFORM_VENDOR,
                                       sizeof(pbuf),
                                       pbuf,
                                       NULL);

            if(!sampleCommon->checkVal(status,
                                       CL_SUCCESS,
                                       "clGetPlatformInfo failed."))
            {
                return SDK_FAILURE;
            }

            platform = platforms[i];
            if (!strcmp(pbuf, "Advanced Micro Devices, Inc.")) 
            {
                break;
            }
        }
        delete[] platforms;
    }

    if(NULL == platform)
    {
        sampleCommon->error("NULL platform found so Exiting Application.");
        return SDK_FAILURE;
    }

    /*
     * If we could find our platform, use it. Otherwise use just available platform.
     */

    cl_context_properties cps[3] = 
    {
        CL_CONTEXT_PLATFORM, 
        (cl_context_properties)platform, 
        0
    };

    context = clCreateContextFromType(
        cps,
        dType,
        NULL,
        NULL,
        &status);

    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateContextFromType failed."))
    {
        return SDK_FAILURE;
    }

    size_t deviceListSize;

    /* First, get the size of device list data */
    status = clGetContextInfo(
        context, 
        CL_CONTEXT_DEVICES, 
        0, 
        NULL, 
        &deviceListSize);
    if(!sampleCommon->checkVal(
        status, 
        CL_SUCCESS,
        "clGetContextInfo failed."))
        return SDK_FAILURE;

    /* Now allocate memory for device list based on the size we got earlier */
    devices = (cl_device_id*)malloc(deviceListSize);
    if(devices==NULL) {
        sampleCommon->error("Failed to allocate memory (devices).");
        return SDK_FAILURE;
    }

    /* Now, get the device list data */
    status = clGetContextInfo(
        context, 
        CL_CONTEXT_DEVICES, 
        deviceListSize, 
        devices, 
        NULL);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clGetContextInfo failed."))
        return SDK_FAILURE;


    /* Create command queue */

    commandQueue = clCreateCommandQueue(
        context,
        devices[0],
        0,
        &status);

    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateCommandQueue failed."))
    {
        return SDK_FAILURE;
    }

    /* Get Device specific Information */
    status = clGetDeviceInfo(
        devices[0],
        CL_DEVICE_MAX_WORK_GROUP_SIZE,
        sizeof(size_t),
        (void*)&maxWorkGroupSize,
        NULL);

    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clGetDeviceInfo CL_DEVICE_MAX_WORK_GROUP_SIZE failed."))
        return SDK_FAILURE;


    status = clGetDeviceInfo(
        devices[0],
        CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
        sizeof(cl_uint),
        (void*)&maxDimensions,
        NULL);

    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clGetDeviceInfo CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS failed."))
        return SDK_FAILURE;


    maxWorkItemSizes = (size_t*)malloc(maxDimensions * sizeof(size_t));

    status = clGetDeviceInfo(
        devices[0],
        CL_DEVICE_MAX_WORK_ITEM_SIZES,
        sizeof(size_t) * maxDimensions,
        (void*)maxWorkItemSizes,
        NULL);

    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clGetDeviceInfo CL_DEVICE_MAX_WORK_ITEM_SIZES failed."))
        return SDK_FAILURE;


    status = clGetDeviceInfo(
        devices[0],
        CL_DEVICE_LOCAL_MEM_SIZE,
        sizeof(cl_ulong),
        (void *)&totalLocalMemory,
        NULL);

    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clGetDeviceInfo CL_DEVICE_LOCAL_MEM_SIZE failed."))
        return SDK_FAILURE;


    /*
    * Create and initialize memory objects
    */

    /* Create memory objects for position */
    currPosX = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        numBodies * sizeof(cl_float),
        0,
        &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateBuffer failed. (oldPos)"))
    {
        return SDK_FAILURE;
    }
    currPosY = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        numBodies * sizeof(cl_float),
        0,
        &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateBuffer failed. (oldPos)"))
    {
        return SDK_FAILURE;
    }
    currPosZ = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        numBodies * sizeof(cl_float),
        0,
        &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateBuffer failed. (oldPos)"))
    {
        return SDK_FAILURE;
    }
    currPosW = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        numBodies * sizeof(cl_float),
        0,
        &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateBuffer failed. (oldPos)"))
    {
        return SDK_FAILURE;
    }

    /* Initialize position buffer */
    status = clEnqueueWriteBuffer(commandQueue,
                                  currPosX,
                                  1,
                                  0,
                                  numBodies * sizeof(cl_float),
                                  posX,
                                  0,
                                  0,
                                  0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clEnqueueWriteBuffer failed. (oldPos)"))
    {
        return SDK_FAILURE;
    }
    status = clEnqueueWriteBuffer(commandQueue,
                                  currPosY,
                                  1,
                                  0,
                                  numBodies * sizeof(cl_float),
                                  posY,
                                  0,
                                  0,
                                  0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clEnqueueWriteBuffer failed. (oldPos)"))
    {
        return SDK_FAILURE;
    }
    status = clEnqueueWriteBuffer(commandQueue,
                                  currPosZ,
                                  1,
                                  0,
                                  numBodies * sizeof(cl_float),
                                  posZ,
                                  0,
                                  0,
                                  0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clEnqueueWriteBuffer failed. (oldPos)"))
    {
        return SDK_FAILURE;
    }
    status = clEnqueueWriteBuffer(commandQueue,
                                  currPosW,
                                  1,
                                  0,
                                  numBodies * sizeof(cl_float),
                                  posW,
                                  0,
                                  0,
                                  0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clEnqueueWriteBuffer failed. (oldPos)"))
    {
        return SDK_FAILURE;
    }


    /* Create memory objects for position */
    newPosX = clCreateBuffer(
		context,
        CL_MEM_READ_WRITE,
        numBodies * sizeof(cl_float),
        0,
        &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateBuffer failed. (newPosX)"))
    {
        return SDK_FAILURE;
    }
    newPosY = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        numBodies * sizeof(cl_float),
        0,
        &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateBuffer failed. (newPosY)"))
    {
        return SDK_FAILURE;
    }
    newPosZ = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        numBodies * sizeof(cl_float),
        0,
        &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateBuffer failed. (newPosZ)"))
    {
        return SDK_FAILURE;
    }
    newPosW = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        numBodies * sizeof(cl_float),
        0,
        &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateBuffer failed. (newPosW)"))
    {
        return SDK_FAILURE;
    }

    /* Create memory objects for velocity */
    currVelX = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        numBodies * sizeof(cl_float),
        0,
        &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateBuffer failed. (oldVel)"))
    {
        return SDK_FAILURE;
    }
    currVelY = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        numBodies * sizeof(cl_float),
        0,
        &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateBuffer failed. (oldVel)"))
    {
        return SDK_FAILURE;
    }
    currVelZ = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        numBodies * sizeof(cl_float),
        0,
        &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateBuffer failed. (oldVel)"))
    {
        return SDK_FAILURE;
    }

    /* Initialize velocity buffer */
    status = clEnqueueWriteBuffer(commandQueue,
                                  currVelX,
                                  1,
                                  0,
                                  numBodies * sizeof(cl_float),
                                  velX,
                                  0,
                                  0,
                                  0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clEnqueueWriteBuffer failed. (oldVel)"))
    {
        return SDK_FAILURE;
    }
    status = clEnqueueWriteBuffer(commandQueue,
                                  currVelY,
                                  1,
                                  0,
                                  numBodies * sizeof(cl_float),
                                  velY,
                                  0,
                                  0,
                                  0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clEnqueueWriteBuffer failed. (oldVel)"))
    {
        return SDK_FAILURE;
    }
    status = clEnqueueWriteBuffer(commandQueue,
                                  currVelZ,
                                  1,
                                  0,
                                  numBodies * sizeof(cl_float),
                                  velZ,
                                  0,
                                  0,
                                  0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clEnqueueWriteBuffer failed. (oldVel)"))
    {
        return SDK_FAILURE;
    }

    /* Create memory objects for velocity */
    newVelX = clCreateBuffer(
        context,
        CL_MEM_READ_ONLY,
        numBodies * sizeof(cl_float),
        0,
        &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateBuffer failed. (newVelX)"))
    {
        return SDK_FAILURE;
    }
    newVelY = clCreateBuffer(
        context,
        CL_MEM_READ_ONLY,
        numBodies * sizeof(cl_float),
        0,
        &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateBuffer failed. (newVelY)"))
    {
        return SDK_FAILURE;
    }
    newVelZ = clCreateBuffer(
        context,
        CL_MEM_READ_ONLY,
        numBodies * sizeof(cl_float),
        0,
        &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateBuffer failed. (newVelZ)"))
    {
        return SDK_FAILURE;
    }

    /* create a CL program using the kernel source */
    //streamsdk::SDKFile kernelFile;
    //std::string kernelPath = sampleCommon->getPath();
    //kernelPath.append("NBodySimple_Kernels.cl");
    //if(!kernelFile.open(kernelPath.c_str()))
    //{
        //std::cout << "Failed to load kernel file : " << kernelPath << std::endl;
        //return SDK_FAILURE;
    //}
    //const char * source = kernelFile.source().c_str();
    const char * source = "NBodySimple_Kernels.bc";
    size_t sourceSize[] = { strlen(source) };
    program = clCreateProgramWithSource(
        context,
        1,
        &source,
        sourceSize,
        &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateProgramWithSource failed."))
        return SDK_FAILURE;

    /* create a cl program executable for all the devices specified */
    status = clBuildProgram(
        program,
        1,
        &devices[0],
        NULL,
        NULL,
        NULL);
    if(status != CL_SUCCESS)
    {
        if(status == CL_BUILD_PROGRAM_FAILURE)
        {
            cl_int logStatus;
            char * buildLog = NULL;
            size_t buildLogSize = 0;
            logStatus = clGetProgramBuildInfo (program, 
                devices[0], 
                CL_PROGRAM_BUILD_LOG, 
                buildLogSize, 
                buildLog, 
                &buildLogSize);
            if(!sampleCommon->checkVal(
                logStatus,
                CL_SUCCESS,
                "clGetProgramBuildInfo failed."))
                return SDK_FAILURE;

            buildLog = (char*)malloc(buildLogSize);
            if(buildLog == NULL)
            {
                sampleCommon->error("Failed to allocate host memory. (buildLog)");
                return SDK_FAILURE;
            }
            memset(buildLog, 0, buildLogSize);

            logStatus = clGetProgramBuildInfo (program, 
                devices[0], 
                CL_PROGRAM_BUILD_LOG, 
                buildLogSize, 
                buildLog, 
                NULL);
            if(!sampleCommon->checkVal(
                logStatus,
                CL_SUCCESS,
                "clGetProgramBuildInfo failed."))
            {
                free(buildLog);
                return SDK_FAILURE;
            }

            std::cout << " \n\t\t\tBUILD LOG\n";
            std::cout << " ************************************************\n";
            std::cout << buildLog << std::endl;
            std::cout << " ************************************************\n";
            free(buildLog);
        }

        if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "clBuildProgram failed."))
            return SDK_FAILURE;
    }

    /* get a kernel object handle for a kernel with the given name */
    kernel = clCreateKernel(
        program,
        "nbody_sim",
        &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateKernel failed."))
    {
        return SDK_FAILURE;
    }

    return SDK_SUCCESS;
}


int 
NBodySimple::setupCLKernels()
{
    cl_int status;

    /* Set appropriate arguments to the kernel */

    /* Particle positions */
    status = clSetKernelArg(
        kernel,
        0,
        sizeof(cl_mem),
        (void*)&currPosX);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (updatedPos)"))
    {
        return SDK_FAILURE;
    }
    status = clSetKernelArg(
        kernel,
        1,
        sizeof(cl_mem),
        (void*)&currPosY);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (updatedPos)"))
    {
        return SDK_FAILURE;
    }
    status = clSetKernelArg(
        kernel,
        2,
        sizeof(cl_mem),
        (void*)&currPosZ);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (updatedPos)"))
    {
        return SDK_FAILURE;
    }
    status = clSetKernelArg(
        kernel,
        3,
        sizeof(cl_mem),
        (void*)&currPosW);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (updatedPos)"))
    {
        return SDK_FAILURE;
    }

    /* Particle velocity */
    status = clSetKernelArg(
        kernel,
        4,
        sizeof(cl_mem),
        (void *)&currVelX);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (updatedVel)"))
    {
        return SDK_FAILURE;
    }
    status = clSetKernelArg(
        kernel,
        5,
        sizeof(cl_mem),
        (void *)&currVelY);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (updatedVel)"))
    {
        return SDK_FAILURE;
    }
    status = clSetKernelArg(
        kernel,
        6,
        sizeof(cl_mem),
        (void *)&currVelZ);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (updatedVel)"))
    {
        return SDK_FAILURE;
    }

    /* numBodies */
    status = clSetKernelArg(
        kernel,
        7,
        sizeof(cl_int),
        (void *)&numBodies);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (numBodies)"))
    {
        return SDK_FAILURE;
    }

    /* time step */
    status = clSetKernelArg(
        kernel,
        8,
        sizeof(cl_float),
        (void *)&delT);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (delT)"))
    {
        return SDK_FAILURE;
    }

    /* upward Pseudoprobability */
    status = clSetKernelArg(
        kernel,
        9,
        sizeof(cl_float),
        (void *)&espSqr);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (espSqr)"))
    {
        return SDK_FAILURE;
    }


    /* local memory */
    status = clSetKernelArg(
        kernel,
        10,
        GROUP_SIZE * sizeof(float),
        NULL);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (localPos)"))
    {
        return SDK_FAILURE;
    }
    status = clSetKernelArg(
        kernel,
        11,
        GROUP_SIZE * sizeof(float),
        NULL);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (localPos)"))
    {
        return SDK_FAILURE;
    }
    status = clSetKernelArg(
        kernel,
        12,
        GROUP_SIZE * sizeof(float),
        NULL);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (localPos)"))
    {
        return SDK_FAILURE;
    }
    status = clSetKernelArg(
        kernel,
        13,
        GROUP_SIZE * sizeof(float),
        NULL);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (localPos)"))
    {
        return SDK_FAILURE;
    }

    /* Particle positions */
    status = clSetKernelArg(
        kernel,
        14,
        sizeof(cl_mem),
        (void*)&newPosX);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (unewPos)"))
    {
        return SDK_FAILURE;
    }
    status = clSetKernelArg(
        kernel,
        15,
        sizeof(cl_mem),
        (void*)&newPosY);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (unewPos)"))
    {
        return SDK_FAILURE;
    }
    status = clSetKernelArg(
        kernel,
        16,
        sizeof(cl_mem),
        (void*)&newPosZ);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (unewPos)"))
    {
        return SDK_FAILURE;
    }
    status = clSetKernelArg(
        kernel,
        17,
        sizeof(cl_mem),
        (void*)&newPosW);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (unewPos)"))
    {
        return SDK_FAILURE;
    }

    /* Particle velocity */
    status = clSetKernelArg(
        kernel,
        18,
        sizeof(cl_mem),
        (void *)&newVelX);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (newVelX)"))
    {
        return SDK_FAILURE;
    }
    status = clSetKernelArg(
        kernel,
        19,
        sizeof(cl_mem),
        (void *)&newVelY);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (newVelY)"))
    {
        return SDK_FAILURE;
    }
    status = clSetKernelArg(
        kernel,
        20,
        sizeof(cl_mem),
        (void *)&newVelZ);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (newVelZ)"))
    {
        return SDK_FAILURE;
    }

    status = clGetKernelWorkGroupInfo(kernel,
        devices[0],
        CL_KERNEL_LOCAL_MEM_SIZE,
        sizeof(cl_ulong),
        &usedLocalMemory,
        NULL);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clGetKernelWorkGroupInfo CL_KERNEL_LOCAL_MEM_SIZE failed."))
    {
        return SDK_FAILURE;
    }

    if(usedLocalMemory > totalLocalMemory)
    {
        std::cout << "Unsupported: Insufficient local memory on device" <<
            std::endl;
        return SDK_FAILURE;
    }

    /* Check group size against group size returned by kernel */
    status = clGetKernelWorkGroupInfo(kernel,
        devices[0],
        CL_KERNEL_WORK_GROUP_SIZE,
        sizeof(size_t),
        &kernelWorkGroupSize,
        0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clGetKernelWorkGroupInfo CL_KERNEL_COMPILE_WORK_GROUP_SIZE failed."))
    {
        return SDK_FAILURE;
    }

    if(groupSize > kernelWorkGroupSize)
    {
        if(!quiet)
        {
            std::cout << "Out of Resources!" << std::endl;
            std::cout << "Group Size specified : " << groupSize << std::endl;
            std::cout << "Max Group Size supported on the kernel : "
                      << kernelWorkGroupSize << std::endl;
            std::cout << "Falling back to " << kernelWorkGroupSize << std::endl;
        }
        groupSize = kernelWorkGroupSize;
    }

    return SDK_SUCCESS;
}

int 
NBodySimple::runCLKernels()
{
    cl_int status;
    cl_event events[1];

    /* 
    * Enqueue a kernel run call.
    */
    size_t globalThreads[] = {numBodies};
    size_t localThreads[] = {groupSize};

    if(localThreads[0] > maxWorkItemSizes[0] ||
       localThreads[0] > maxWorkGroupSize)
    {
        std::cout << "Unsupported: Device"
            "does not support requested number of work items.";
        return SDK_FAILURE;
    }

    status = clEnqueueNDRangeKernel(
        commandQueue,
        kernel,
        1,
        NULL,
        globalThreads,
        localThreads,
        0,
        NULL,
        NULL);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clEnqueueNDRangeKernel failed."))
    {
        return SDK_FAILURE;
    }

    status = clFinish(commandQueue);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clFinish failed."))
    {
        return SDK_FAILURE;
    }

    /* Copy data from new to old */
    status = clEnqueueCopyBuffer(commandQueue,
                                 newPosX,
                                 currPosX,
                                 0,
                                 0,
                                 sizeof(cl_float) * numBodies,
                                 0,
                                 0,
                                 0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clEnqueueCopyBuffer failed.(newPosX->oldPosX)"))
    {
        return SDK_FAILURE;
    }
    status = clEnqueueCopyBuffer(commandQueue,
                                 newPosY,
                                 currPosY,
                                 0,
                                 0,
                                 sizeof(cl_float) * numBodies,
                                 0,
                                 0,
                                 0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clEnqueueCopyBuffer failed.(newPosY->oldPosY)"))
    {
        return SDK_FAILURE;
    }
    status = clEnqueueCopyBuffer(commandQueue,
                                 newPosZ,
                                 currPosZ,
                                 0,
                                 0,
                                 sizeof(cl_float) * numBodies,
                                 0,
                                 0,
                                 0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clEnqueueCopyBuffer failed.(newPosZ->oldPosZ)"))
    {
        return SDK_FAILURE;
    }
    status = clEnqueueCopyBuffer(commandQueue,
                                 newPosW,
                                 currPosW,
                                 0,
                                 0,
                                 sizeof(cl_float) * numBodies,
                                 0,
                                 0,
                                 0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clEnqueueCopyBuffer failed.(newPosW->oldPosW)"))
    {
        return SDK_FAILURE;
    }

    status = clEnqueueCopyBuffer(commandQueue,
                                 newVelX,
                                 currVelX,
                                 0,
                                 0,
                                 sizeof(cl_float) * numBodies,
                                 0,
                                 0,
                                 0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clEnqueueCopyBuffer failed.(newVel->oldVels)"))
    {
        return SDK_FAILURE;
    }
    status = clEnqueueCopyBuffer(commandQueue,
                                 newVelY,
                                 currVelY,
                                 0,
                                 0,
                                 sizeof(cl_float) * numBodies,
                                 0,
                                 0,
                                 0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clEnqueueCopyBuffer failed.(newVel->oldVels)"))
    {
        return SDK_FAILURE;
    }
    status = clEnqueueCopyBuffer(commandQueue,
                                 newVelZ,
                                 currVelZ,
                                 0,
                                 0,
                                 sizeof(cl_float) * numBodies,
                                 0,
                                 0,
                                 0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clEnqueueCopyBuffer failed.(newVel->oldVels)"))
    {
        return SDK_FAILURE;
    }

    status = clFinish(commandQueue);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clFinish failed."))
    {
        return SDK_FAILURE;
    }

    /* Enqueue readBuffer*/
    status = clEnqueueReadBuffer(
        commandQueue,
        currPosX,
        CL_TRUE,
        0,
        numBodies* sizeof(cl_float),
        posX,
        0,
        NULL,
        &events[0]);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clEnqueueReadBuffer failed."))
        return SDK_FAILURE;
    status = clEnqueueReadBuffer(
        commandQueue,
        currPosY,
        CL_TRUE,
        0,
        numBodies* sizeof(cl_float),
        posY,
        0,
        NULL,
        &events[0]);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clEnqueueReadBuffer failed."))
        return SDK_FAILURE;
    status = clEnqueueReadBuffer(
        commandQueue,
        currPosZ,
        CL_TRUE,
        0,
        numBodies* sizeof(cl_float),
        posZ,
        0,
        NULL,
        &events[0]);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clEnqueueReadBuffer failed."))
        return SDK_FAILURE;
    status = clEnqueueReadBuffer(
        commandQueue,
        currPosW,
        CL_TRUE,
        0,
        numBodies* sizeof(cl_float),
        posW,
        0,
        NULL,
        &events[0]);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clEnqueueReadBuffer failed."))
        return SDK_FAILURE;

    /* Wait for the read buffer to finish execution */
    status = clWaitForEvents(1, &events[0]);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clWaitForEvents failed."))
        return SDK_FAILURE;

    clReleaseEvent(events[0]);

    return SDK_SUCCESS;
}

/*
* n-body simulation on cpu
*/
void 
NBodySimple::nBodyCPUReference()
{
    //Iterate for all samples
    for(int i = 0; i < numBodies; ++i)
    {
        int myIndex = i;
        float acc[3] = {0.0f, 0.0f, 0.0f};
        for(int j = 0; j < numBodies; ++j)
        {
            float r[3];
            int index = j;

			r[0] = refPosX[index] - refPosX[myIndex];
			r[1] = refPosY[index] - refPosY[myIndex];
			r[2] = refPosZ[index] - refPosZ[myIndex];
            
			float distSqr = r[0] * r[0] + r[1] * r[1] + r[2] * r[2];

            float invDist = 1.0f / sqrt(distSqr + espSqr);
            float invDistCube =  invDist * invDist * invDist;
            float s = refPosW[index] * invDistCube;

			acc[0] += s * r[0];
			acc[1] += s * r[1];
			acc[2] += s * r[2];
        }

		refPosX[myIndex] += refVelX[myIndex] * delT + 0.5f * acc[0] * delT * delT;
		refPosY[myIndex] += refVelY[myIndex] * delT + 0.5f * acc[1] * delT * delT;
		refPosZ[myIndex] += refVelZ[myIndex] * delT + 0.5f * acc[2] * delT * delT;
		refVelX[myIndex] += acc[0] * delT;
		refVelY[myIndex] += acc[1] * delT;
		refVelZ[myIndex] += acc[2] * delT;
    }
}

int
NBodySimple::initialize()
{
    /* Call base class Initialize to get default configuration */
    if(!this->SDKSample::initialize())
        return SDK_FAILURE;

    streamsdk::Option *num_particles = new streamsdk::Option;
    if(!num_particles)
    {
        std::cout << "error. Failed to allocate memory (num_particles)\n";
        return SDK_FAILURE;
    }

    num_particles->_sVersion = "x";
    num_particles->_lVersion = "particles";
    num_particles->_description = "Number of particles";
    num_particles->_type = streamsdk::CA_ARG_INT;
    num_particles->_value = &numParticles;

    sampleArgs->AddOption(num_particles);
    delete num_particles;

    streamsdk::Option *num_iterations = new streamsdk::Option;
    if(!num_iterations)
    {
        std::cout << "error. Failed to allocate memory (num_iterations)\n";
        return SDK_FAILURE;
    }

    num_iterations->_sVersion = "i";
    num_iterations->_lVersion = "iterations";
    num_iterations->_description = "Number of iterations";
    num_iterations->_type = streamsdk::CA_ARG_INT;
    num_iterations->_value = &iterations;

    sampleArgs->AddOption(num_iterations);
    delete num_iterations;

    return SDK_SUCCESS;
}

int
NBodySimple::setup()
{
    if(setupNBodySimple() != SDK_SUCCESS)
        return SDK_FAILURE;

    int timer = sampleCommon->createTimer();
    sampleCommon->resetTimer(timer);
    sampleCommon->startTimer(timer);

    if(setupCL() != SDK_SUCCESS)
        return SDK_FAILURE;

    sampleCommon->stopTimer(timer);
    /* Compute setup time */
    setupTime = (double)(sampleCommon->readTimer(timer));

    display = !quiet && !verify;

    return SDK_SUCCESS;
}

/** 
* @brief Initialize GL 
*/
void 
GLInit()
{
    glClearColor(0.0 ,0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);	
    glLoadIdentity();
}

/** 
* @brief Glut Idle function
*/
void 
idle()
{
    glutPostRedisplay();
}

/** 
* @brief Glut reshape func
* 
* @param w numParticles of OpenGL window
* @param h height of OpenGL window 
*/
void 
reShape(int w,int h)
{
    glViewport(0, 0, w, h);

    glViewport(0, 0, w, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluPerspective(45.0f, w/h, 1.0f, 1000.0f);
    gluLookAt (0.0, 0.0, -2.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
}

/** 
* @brief OpenGL display function
*/
void displayfunc()
{
    glClearColor(0.0 ,0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    glPointSize(1.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);

    glColor3f(1.0f,0.6f,0.0f);

    //Calling kernel for calculatig subsequent positions
    ((NBodySimple*)me)->runCLKernels();

    glBegin(GL_POINTS);
    for(int i=0; i < numBodies; ++i)
    {
        //divided by 300 just for scaling
        glVertex3d(posX[i]/300, posY[i]/300, posZ[i]/300);
    }
    glEnd();

    glFlush();
    glutSwapBuffers();
}

/* keyboard function */
void
keyboardFunc(unsigned char key, int mouseX, int mouseY)
{
    switch(key)
    {
        /* If the user hits escape or Q, then exit */
        /* ESCAPE_KEY = 27 */
    case 27:
    case 'q':
    case 'Q':
        {
            if(((NBodySimple*)me)->cleanup() != SDK_SUCCESS)
                exit(1);
            else
                exit(0);
        }
    default:
        break;
    }
}


int 
NBodySimple::run()
{
    /* Arguments are set and execution call is enqueued on command buffer */
    if(setupCLKernels() != SDK_SUCCESS)
    {
        return SDK_FAILURE;
    }

    if(verify || timing)
    {
        int timer = sampleCommon->createTimer();
        sampleCommon->resetTimer(timer);
        sampleCommon->startTimer(timer);

        for(int i = 0; i < iterations; ++i)
        {
            runCLKernels();
        }

        sampleCommon->stopTimer(timer);
        /* Compute kernel time */
        kernelTime = (double)(sampleCommon->readTimer(timer)) / iterations;
    }

    if(!quiet)
    {
        sampleCommon->printArray<cl_float>("Output", posX, numBodies, 1);
        sampleCommon->printArray<cl_float>("Output", posY, numBodies, 1);
        sampleCommon->printArray<cl_float>("Output", posZ, numBodies, 1);
        sampleCommon->printArray<cl_float>("Output", posW, numBodies, 1);
    }

    return SDK_SUCCESS;
}

int
NBodySimple::verifyResults()
{
    if(verify)
    {
        /* reference implementation
        * it overwrites the input array with the output
        */

        refPosX = (cl_float*)malloc(numBodies * sizeof(cl_float));
        if(refPosX == NULL)
        { 
            sampleCommon->error("Failed to allocate host memory. (refPosX)");
            return SDK_FAILURE;
        }
        refPosY = (cl_float*)malloc(numBodies * sizeof(cl_float));
        if(refPosY == NULL)
        { 
            sampleCommon->error("Failed to allocate host memory. (refPosY)");
            return SDK_FAILURE;
        }
        refPosZ = (cl_float*)malloc(numBodies * sizeof(cl_float));
        if(refPosZ == NULL)
        { 
            sampleCommon->error("Failed to allocate host memory. (refPosZ)");
            return SDK_FAILURE;
        }
        refPosW = (cl_float*)malloc(numBodies * sizeof(cl_float));
        if(refPosW == NULL)
        { 
            sampleCommon->error("Failed to allocate host memory. (refPosW)");
            return SDK_FAILURE;
        }

        refVelX = (cl_float*)malloc(numBodies * sizeof(cl_float));
        refVelY = (cl_float*)malloc(numBodies * sizeof(cl_float));
        refVelZ = (cl_float*)malloc(numBodies * sizeof(cl_float));
        if(refVelX == NULL || refVelY == NULL || refVelZ == NULL)
        { 
            sampleCommon->error("Failed to allocate host memory. (refVel)");
            return SDK_FAILURE;
        }

        memcpy(refPosX, initPosX, numBodies * sizeof(cl_float));
        memcpy(refPosY, initPosY, numBodies * sizeof(cl_float));
        memcpy(refPosZ, initPosZ, numBodies * sizeof(cl_float));
        memcpy(refPosW, initPosW, numBodies * sizeof(cl_float));
        memcpy(refVelX, initVelX, numBodies * sizeof(cl_float));
        memcpy(refVelY, initVelY, numBodies * sizeof(cl_float));
        memcpy(refVelZ, initVelZ, numBodies * sizeof(cl_float));

        for(int i = 0; i < iterations; ++i)
        {
            nBodyCPUReference();
        }

        /* compare the results and see if they match */
        if(sampleCommon->compare(posX, refPosX, numBodies, 0.00001) &&
           sampleCommon->compare(posY, refPosY, numBodies, 0.00001) &&
           sampleCommon->compare(posZ, refPosZ, numBodies, 0.00001) &&
           sampleCommon->compare(posW, refPosW, numBodies, 0.00001))
        {
            std::cout << "Passed!\n";
            return SDK_SUCCESS;
        }
        else
        {
            std::cout << "Failed!\n";
            return SDK_FAILURE;
        }
    }

    return SDK_SUCCESS;
}

void 
NBodySimple::printStats()
{
    std::string strArray[4] = 
    {
        "Particles", 
        "Iterations", 
        "Time(sec)", 
        "kernelTime(sec)"
    };

    std::string stats[4];
    totalTime = setupTime + kernelTime;

    stats[0] = sampleCommon->toString(numParticles, std::dec);
    stats[1] = sampleCommon->toString(iterations, std::dec);
    stats[2] = sampleCommon->toString(totalTime, std::dec);
    stats[3] = sampleCommon->toString(kernelTime, std::dec);

    this->SDKSample::printStats(strArray, stats, 4);
}

int
NBodySimple::cleanup()
{
    /* Releases OpenCL resources (Context, Memory etc.) */
    cl_int status;

    status = clReleaseKernel(kernel);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseKernel failed."))
    {
        return SDK_FAILURE;
    }

    status = clReleaseProgram(program);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseProgram failed."))
    {
        return SDK_FAILURE;
    }

    status = clReleaseMemObject(currPosX);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseMemObject failed."))
    {
        return SDK_FAILURE;
    }
    status = clReleaseMemObject(currPosY);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseMemObject failed."))
    {
        return SDK_FAILURE;
    }
    status = clReleaseMemObject(currPosZ);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseMemObject failed."))
    {
        return SDK_FAILURE;
    }
    status = clReleaseMemObject(currPosW);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseMemObject failed."))
    {
        return SDK_FAILURE;
    }

    status = clReleaseMemObject(currVelX);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseMemObject failed."))
    {
        return SDK_FAILURE;
    }
    status = clReleaseMemObject(currVelY);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseMemObject failed."))
    {
        return SDK_FAILURE;
    }
    status = clReleaseMemObject(currVelZ);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseMemObject failed."))
    {
        return SDK_FAILURE;
    }

    status = clReleaseCommandQueue(commandQueue);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseCommandQueue failed."))
    {
        return SDK_FAILURE;
    }

    status = clReleaseContext(context);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseContext failed."))
    {
        return SDK_FAILURE;
    }

    return SDK_SUCCESS;
}

NBodySimple::~NBodySimple()
{
    /* release program resources */
    if(initPosX)
    {
        free(initPosX);
        initPosX = NULL;
    }
    if(initPosY)
    {
        free(initPosY);
        initPosY = NULL;
    }
    if(initPosZ)
    {
        free(initPosZ);
        initPosZ = NULL;
    }
    if(initPosW)
    {
        free(initPosW);
        initPosW = NULL;
    }

    if(initVelX)
    {
        free(initVelX);
        initVelX = NULL;
    }
    if(initVelY)
    {
        free(initVelY);
        initVelY = NULL;
    }
    if(initVelZ)
    {
        free(initVelZ);
        initVelZ = NULL;
    }

    if(posX)
    {
#if defined (_WIN32)
        _aligned_free(posX);
#else
        free(posX);
#endif
        posX = NULL;
    }
    if(posY)
    {
#if defined (_WIN32)
        _aligned_free(posY);
#else
        free(posY);
#endif
        posY = NULL;
    }
    if(posZ)
    {
#if defined (_WIN32)
        _aligned_free(posZ);
#else
        free(posZ);
#endif
        posZ = NULL;
    }
    if(posW)
    {
#if defined (_WIN32)
        _aligned_free(posW);
#else
        free(posW);
#endif
        posW = NULL;
    }
    if(velX)
    {
#if defined (_WIN32)
        _aligned_free(velX);
#else
        free(velX);
#endif
        velX = NULL;
    }
    if(velY)
    {
#if defined (_WIN32)
        _aligned_free(velY);
#else
        free(velY);
#endif
        velY = NULL;
    }
    if(velZ)
    {
#if defined (_WIN32)
        _aligned_free(velZ);
#else
        free(velZ);
#endif
        velZ = NULL;
    }

    if(devices)
    {
        free(devices);
        devices = NULL;
    }

    if(refPosX)
    {
        free(refPosX);
        refPosX = NULL;
    }
    if(refPosY)
    {
        free(refPosY);
        refPosY = NULL;
    }
    if(refPosZ)
    {
        free(refPosZ);
        refPosZ = NULL;
    }
    if(refPosW)
    {
        free(refPosW);
        refPosW = NULL;
    }

    if(refVelX)
    {
        free(refVelX);
        refVelX = NULL;
    }
    if(refVelY)
    {
        free(refVelY);
        refVelY = NULL;
    }
    if(refVelZ)
    {
        free(refVelZ);
        refVelZ = NULL;
    }

    if(maxWorkItemSizes)
    {
        free(maxWorkItemSizes);
        maxWorkItemSizes = NULL;
    }
}


int 
main(int argc, char * argv[])
{
    NBodySimple clNBodySimple("OpenCL NBodySimple");
    me = &clNBodySimple;

    if(clNBodySimple.initialize() != SDK_SUCCESS)
        return SDK_FAILURE;
    if(!clNBodySimple.parseCommandLine(argc, argv))
        return SDK_FAILURE;
    if(clNBodySimple.setup() != SDK_SUCCESS)
        return SDK_FAILURE;
    if(clNBodySimple.run() != SDK_SUCCESS)
        return SDK_FAILURE;
    if(clNBodySimple.verifyResults() != SDK_SUCCESS)
        return SDK_FAILURE;

    clNBodySimple.printStats();

    if(display)
    {
        // Run in  graphical window if requested 
        glutInit(&argc, argv);
        glutInitWindowPosition(100,10);
        glutInitWindowSize(600,600); 
        glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
        glutCreateWindow("nbody simulation"); 
        GLInit(); 
        glutDisplayFunc(displayfunc); 
        glutReshapeFunc(reShape);
        glutIdleFunc(idle); 
        glutKeyboardFunc(keyboardFunc);
        glutMainLoop();
    }

    if(clNBodySimple.cleanup()!=SDK_SUCCESS)
        return SDK_FAILURE;

    return SDK_SUCCESS;
}
