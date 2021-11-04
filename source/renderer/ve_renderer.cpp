//
// Created by hlahm on 2021-10-21.
//

#include "ve_renderer.h"

static struct Ve_renderer renderer = {};
static std::vector<MeshObject> object_list = {};
static std::vector<Camera> camera_list = {};
static struct Lights light_list = {};
static struct Gui gui;

static void initGUI()
{

        //this initializes the core structures of imgui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForVulkan(ve_window.window, true);
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = vulkanBE.instance;
        init_info.PhysicalDevice = vulkanBE.mainDevices.physicalDevice;
        init_info.Device = vulkanBE.mainDevices.device;
        init_info.QueueFamily = vulkanBE.queueFamilyIndices.graphicsFamily;
        init_info.Queue = vulkanBE.graphicsQueue;
        init_info.PipelineCache = VK_NULL_HANDLE;
        init_info.DescriptorPool = vulkanBE.descriptors.imGuiDescriptorPool;
        init_info.Allocator = nullptr;
        init_info.MinImageCount = 2;
        init_info.ImageCount = MAX_FRAME_DRAWS;
        init_info.CheckVkResultFn = check_vk_result;
        init_info.Subpass = 0;
        ImGui_ImplVulkan_Init(&init_info, vulkanBE.renderPass);

        //execute a gpu command to upload imgui font textures
        // Upload Fonts
        {

            // Use any command queue
            VkCommandBuffer command_buffer = vulkanBE.commandBuffers[0];

            VkResult err = vkResetCommandPool(vulkanBE.mainDevices.device, vulkanBE.pools.graphicsCommandPool, 0);
            check_vk_result(err);
            VkCommandBufferBeginInfo begin_info = {};
            begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            err = vkBeginCommandBuffer(command_buffer, &begin_info);
            check_vk_result(err);

            ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

            err = vkEndCommandBuffer(command_buffer);
            check_vk_result(err);

            //err = vkDeviceWaitIdle(device.device());
            //check_vk_result(err);

            VkSubmitInfo end_info = {};
            end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            end_info.commandBufferCount = 1;
            end_info.pCommandBuffers = &command_buffer;

            err = vkQueueSubmit(vulkanBE.graphicsQueue, 1, &end_info, VK_NULL_HANDLE);
            check_vk_result(err);

            err = vkDeviceWaitIdle(vulkanBE.mainDevices.device);
            check_vk_result(err);

            ImGui_ImplVulkan_DestroyFontUploadObjects();
        }
}

static void imGuiParametersSetup() {

    ImGui::Begin("Simple Render Engine!");                          // Create a window called "Hello, world!" and append into it.

    //ImGui::SetWindowSize(ImVec2(350.0f,350.0f),0);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

static void create_texture(struct MeshObject* obj, char* filename)
{
    //create texture Image then store it's index array
    int textureImageLoc = create_texture_image(filename);

    //create texture image view
    allTextures[textureImageLoc].textureImageView = createImageView(allTextures[textureImageLoc].textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);

    //TODO:create descriptor set here
    int descriptorLoc = createTextureDescriptor(allTextures[textureImageLoc].textureImageView);

    obj->textureID = descriptorLoc;
    obj->pObj.isTex = 1;
}

static void loadModels()
{
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f,0.0f,0.0f));

    struct MeshObject checkerBoard = checkerPlane(glm::rotate(GLM_MAT4_IDENTITY, glm::radians(-90.0f), glm::vec3(1.0f,0.0f,0.0f)),
            40,
            glm::vec3(0.4f,0.4f,0.4f),
            glm::vec3(0.2f,0.2f,0.2f));
    struct MeshObject cubeObj = cube(glm::translate(glm::mat4(1.5f),(vec3){0.0f,1.5f,0.0f}), (vec3){1.0f,0.0f,1.0f});
    struct MeshObject plane1 = plane(glm::translate(glm::mat4(1.5f),(vec3){0.0f,1.0f,0.0f}), (vec3){1.0f,0.8f,0.0f});
    struct MeshObject icosa2 = icosahedron(glm::translate(glm::mat4(1.0f),(vec3){0.0f,0.0f,0.0f}), (vec3){0.8f,0.6f,0.0f},10);
    //icosa2.pObj.M[3][1]+=2;

    //icosa.pipelineFlag = 1;
    //plane1.pipelineFlag = 1;
    create_texture(&plane1, (char*)"giraffe.jpg");
    create_texture(&icosa2, (char*)"giraffe.jpg");

    checkerBoard.textureID = 0;
    cubeObj.textureID = 0;
    cubeObj.pObj.isTex = 1;

    //add objects to the object list
    //object_list.push_back(checkerBoard);
    object_list.push_back(icosa2);

}

static void loadCameras()
{
    float aspectRatio = (float)vulkanBE.imageInfo.extent.width/(float)vulkanBE.imageInfo.extent.height;

    struct Camera cam1 = {};

    cam1.projection = glm::perspective(glm::radians(50.0f), aspectRatio, 0.1f, 100.0f);
    cam1.view = glm::lookAt(glm::vec3(0.0f,3.0f,6.0f), glm::vec3(0.0f), glm::vec3(0.0f,1.0f,0.0f));

    //cam1.projection[1][1] *= -1; //we need to infer the y coordinate because vulkan
    camera_list.push_back(cam1);
}

static void loadLights()
{
    light_list.light1 = glm::vec4(4.0f,5.0f,4.0,1.0f);
    light_list.light2 = glm::vec4(-8.0f,5.0f,-3.0f,1.0f);
}

static void createVertexBuffer(struct MeshObject* meshObj)
{
    //std::vector<struct Vertex> vertices;
    //std::vector<uint32_t> indices;
    //getVertandIndx(meshObj, &vertices, &indices);

    VkDeviceSize bufferSize = sizeof(struct Vertex) * meshObj->vertices.size();

    //temporary buffer to stage vertex data before transfering to GPU
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    createBuffer(bufferSize,VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 &stagingBuffer, &stagingBufferMemory);

    //map memory to vertexbuffer
    void* data;                                                                                           //1. create a pointer a point in normal memory;
    vkMapMemory(vulkanBE.mainDevices.device, stagingBufferMemory, 0, bufferSize, 0 , &data);   //2. map the vertex buffer memory to that point.
    memcpy(data, meshObj->vertices.data(), (size_t)bufferSize);                                                  //3. copies memory from vertices vector to the point in memory
    vkUnmapMemory(vulkanBE.mainDevices.device, stagingBufferMemory);                                      //4. unmap the vertex memory

    //create buffer to TRANSFER_DST_BIT to mark as recipient for transfer data;
    //buffer memory is to be device local bit meaning memory is on the GPU and only accessible by it.
    createBuffer(bufferSize,VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                 &meshObj->vertexBuffer, &meshObj->vertexBufferMemory);

    //copy staging buffer to gpu memory buffer
    copyBuffer(stagingBuffer, meshObj->vertexBuffer, bufferSize);

    //clean up staging buffer
    vkDestroyBuffer(vulkanBE.mainDevices.device, stagingBuffer, nullptr);
    vkFreeMemory(vulkanBE.mainDevices.device, stagingBufferMemory, nullptr);
}

static void createIndexBuffer(struct MeshObject* meshObj)
{

    //get size of buffer needed ofr indices
    VkDeviceSize bufferSize = sizeof(uint32_t) * meshObj->indices.size();

    //temporary buffer to stage index data before transfering to GPU
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    createBuffer(bufferSize,VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 &stagingBuffer, &stagingBufferMemory);

    //map memory to indexbuffer
    void* data;                                                                                           //1. create a pointer a point in normal memory;
    vkMapMemory(vulkanBE.mainDevices.device, stagingBufferMemory, 0, bufferSize, 0 , &data);   //2. map the index buffer memory to that point.
    memcpy(data, meshObj->indices.data(), (size_t)bufferSize);                                                   //3. copies memory from indices vector to the point in memory
    vkUnmapMemory(vulkanBE.mainDevices.device, stagingBufferMemory);                                      //4. unmap the index memory

    //Create buffer for index data on GPU access only area
    createBuffer(bufferSize,VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                 &meshObj->indexBuffer, &meshObj->indexBufferMemory);

    //Copy from staging buffer to GPU access buffer
    copyBuffer(stagingBuffer, meshObj->indexBuffer, bufferSize);

    //Destroy + release staging buffer resources
    vkDestroyBuffer(vulkanBE.mainDevices.device, stagingBuffer, nullptr);
    vkFreeMemory(vulkanBE.mainDevices.device, stagingBufferMemory, nullptr);

}

static void createUniformBuffers()
{
    //buffer size will be the size of all three variables (will offset to access)
    VkDeviceSize bufferSize = sizeof(struct Camera);
    VkDeviceSize light_bufferSize = sizeof(struct Lights);

    //one uniform buffer for each image (and by extension, command buffer)
    vulkanBE.descriptors.uniformBuffer = (VkBuffer*)realloc(vulkanBE.descriptors.uniformBuffer, MAX_IMAGE * sizeof(VkBuffer));
    vulkanBE.descriptors.uniformBufferMemory = (VkDeviceMemory*)realloc(vulkanBE.descriptors.uniformBufferMemory, MAX_IMAGE * sizeof(VkDeviceMemory));

    vulkanBE.descriptors.light_UB = (VkBuffer*)realloc(vulkanBE.descriptors.light_UB, MAX_IMAGE * sizeof(VkBuffer));
    vulkanBE.descriptors.light_UBM = (VkDeviceMemory*)realloc(vulkanBE.descriptors.light_UBM, MAX_IMAGE * sizeof(VkDeviceMemory));

    //Create uniform buffer
    for(size_t i = 0; i < MAX_IMAGE; i++)
    {
        createBuffer(bufferSize,VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     &vulkanBE.descriptors.uniformBuffer[i], &vulkanBE.descriptors.uniformBufferMemory[i]);

        createBuffer(light_bufferSize,VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     &vulkanBE.descriptors.light_UB[i], &vulkanBE.descriptors.light_UBM[i]);
    }

    //bind the descriptor set buffer
    for(size_t i = 0; i < MAX_IMAGE; i++)
    {
        //buffer info and data offset info
        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = vulkanBE.descriptors.uniformBuffer[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(struct Camera);

        //buffer info and data offset info
        VkDescriptorBufferInfo lightbufferInfo = {};
        lightbufferInfo.buffer = vulkanBE.descriptors.light_UB[i];
        lightbufferInfo.offset = 0;
        lightbufferInfo.range = sizeof(struct Lights);

        VkDescriptorBufferInfo bufferInfos[] = {bufferInfo, lightbufferInfo};

        //data about connection between binding and buffer
        VkWriteDescriptorSet cameraSetWrite = {};
        cameraSetWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        cameraSetWrite.dstSet = vulkanBE.descriptors.descriptorSet[i];
        cameraSetWrite.dstBinding = 0; //the destination binding in the shader
        cameraSetWrite.dstArrayElement = 0; //index in array to update
        cameraSetWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        cameraSetWrite.descriptorCount = 2; //amount to update
        cameraSetWrite.pBufferInfo = bufferInfos;

        //update the descriptor set buffer binding info
        vkUpdateDescriptorSets(vulkanBE.mainDevices.device, 1, &cameraSetWrite, 0, nullptr);
    }
}

static void updateUniformBuffers(uint32_t imgIndex)
{
    for(size_t c = 0; c<camera_list.size(); c++)
    {
        void *data;
        vkMapMemory(vulkanBE.mainDevices.device, vulkanBE.descriptors.uniformBufferMemory[imgIndex], 0,
                    sizeof(struct Camera), 0, &data);
        memcpy(data, &camera_list[c], sizeof(struct Camera));
        vkUnmapMemory(vulkanBE.mainDevices.device, vulkanBE.descriptors.uniformBufferMemory[imgIndex]);
    }

    //camera data update
    void *data;
    vkMapMemory(vulkanBE.mainDevices.device, vulkanBE.descriptors.light_UBM[imgIndex], 0,
                sizeof(struct Lights), 0, &data);
    memcpy(data, &light_list, sizeof(struct Lights));
    vkUnmapMemory(vulkanBE.mainDevices.device, vulkanBE.descriptors.light_UBM[imgIndex]);
}

static void updateModel()
{

    float angle = 0.0f;
    float deltaTime;
    auto now = (float)glfwGetTime();

    angle = now;
    if(angle>=360.0f){angle-=360.0f;}

    mat4 model = GLM_MAT4_IDENTITY_INIT;
    //glm_translate(model, (vec3){0.0f,sin(angle),0.0f});
    model = glm::rotate(model, angle, glm::vec3(0.0f,1.0f,0.0f));
    //model = glm::rotate(model, angle * 0.7f, glm::vec3(0.0f,1.0f,0.0f));
    object_list[0].pObj.M = model;
    //glm_mat4_copy(model, object_list.objects[0].pObj.M);
    //glm_mat4_copy(model, object_list.objects[1].pObj.M);
}

static void destroyVertexBuffer(struct MeshObject* meshObj)
{
    vkDestroyBuffer(vulkanBE.mainDevices.device, meshObj->vertexBuffer, nullptr);
    vkFreeMemory(vulkanBE.mainDevices.device, meshObj->vertexBufferMemory, nullptr);
}

static void destroyIndexBuffer(struct MeshObject* meshObj)
{
    vkDestroyBuffer(vulkanBE.mainDevices.device, meshObj->indexBuffer, nullptr);
    vkFreeMemory(vulkanBE.mainDevices.device, meshObj->indexBufferMemory, nullptr);
}



static void cleanUpRenderer()
{
    vkDeviceWaitIdle(vulkanBE.mainDevices.device);
    for(auto & obj : object_list)
    {
        destroyVertexBuffer(&obj);
        destroyIndexBuffer(&obj);
    }

}

static void recordCommand(uint32_t currentImage){

    //information about how to begin each command buffers
    VkCommandBufferBeginInfo bufferBeginInfo = {};
    bufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    //bufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT; //buffer can be resubmitted when it has already been submitted and is awaiting execution.

    //information about how to begin a renderpass (only needed for graphical application)
    VkRenderPassBeginInfo renderPassBeginInfo = {};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = vulkanBE.renderPass;
    renderPassBeginInfo.renderArea.offset.x = 0;
    renderPassBeginInfo.renderArea.offset.y = 0;
    renderPassBeginInfo.renderArea.extent = vulkanBE.imageInfo.extent; //size of region to run renderpass on.

    //assigns to each attachment. clears attachment[1] to cleavalues[1]
    VkClearValue clearValues[2] = {}; //add depth attachment lear value
    clearValues[0].color = {0.05f,0.05f,0.05f,1.0f};
    clearValues[1].depthStencil.depth = 1.0f;

    renderPassBeginInfo.pClearValues = clearValues;
    renderPassBeginInfo.clearValueCount = 2;



    //select the framebuffer to render to
    renderPassBeginInfo.framebuffer = vulkanBE.swapChainFrameBuffers[currentImage];

    //start recording command with command buffer
    VkResult result = vkBeginCommandBuffer(vulkanBE.commandBuffers[currentImage],&bufferBeginInfo);
    if(result != VK_SUCCESS){
        fprintf(stderr, "failed to start recording a command buffer\n");
        exit(1);
    }

    vkCmdBeginRenderPass(vulkanBE.commandBuffers[currentImage], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE); // all the commands will be primary

        for(int j = 0; j < object_list.size(); j++)
        {
            //bind pipeline to be used in renderpass
            vkCmdBindPipeline(vulkanBE.commandBuffers[currentImage], VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanBE.graphicsPipeline[object_list[j].pipelineFlag]);

            VkBuffer vertexBuffers[] = {object_list[j].vertexBuffer};      //buffers to bind
            VkDeviceSize offsets[] = {0};                           //offset into buffer being bound
            vkCmdBindVertexBuffers(vulkanBE.commandBuffers[currentImage], 0, 1, vertexBuffers, offsets);

            //binding the index buffer with 0 offset using the uint32type
            vkCmdBindIndexBuffer(vulkanBE.commandBuffers[currentImage], object_list[j].indexBuffer, 0, VK_INDEX_TYPE_UINT32);

            //push pushconstat
            vkCmdPushConstants(vulkanBE.commandBuffers[currentImage], vulkanBE.pipelineLayout[object_list[j].pipelineFlag], VK_SHADER_STAGE_VERTEX_BIT, 0, vulkanBE.descriptors.pushConstantRange.size, &object_list[j].pObj);

            VkDescriptorSet descriptorSetGroup[2] = {vulkanBE.descriptors.descriptorSet[currentImage], vulkanBE.descriptors.samplerDescriptorSet[object_list[j].textureID]};
            int numDescriptorSet = 2;
            if(object_list[j].textureID == -1) {
                numDescriptorSet = 1;
            }
            //bind descriptor set
            vkCmdBindDescriptorSets(vulkanBE.commandBuffers[currentImage], VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanBE.pipelineLayout[object_list[j].pipelineFlag], 0, numDescriptorSet, descriptorSetGroup, 0, nullptr);

            //number of times to draw/run the shader/pipeline.
            vkCmdDrawIndexed(vulkanBE.commandBuffers[currentImage], object_list[j].indices.size(), 1, 0, 0, 0);
        }

    ImGui_ImplVulkan_RenderDrawData(gui.draw_data, vulkanBE.commandBuffers[currentImage]);

    vkCmdEndRenderPass(vulkanBE.commandBuffers[currentImage]); // all the commands will be primary


    result = vkEndCommandBuffer(vulkanBE.commandBuffers[currentImage]);
    if(result != VK_SUCCESS){
        fprintf(stderr, "failed to stop recording a command buffer\n");
        exit(1);
    }

}

static void draw()
{
    //1-get the next available image to draw to and set something to signal when we're finished with the image (a semaphore)
    //2-submit command buffer to queue for execution. making sure it waits for the image to be signalled as available before drawing and signals when it has finished rendering.
    //3-present image to screen when it has signaled finished rendering.

    vkWaitForFences(vulkanBE.mainDevices.device, 1, &vulkanBE.sync.drawFences[currentFrame], VK_TRUE, INT_MAX); //freeze the code until the fence is signaled
    vkResetFences(vulkanBE.mainDevices.device, 1, &vulkanBE.sync.drawFences[currentFrame]); // manually unsignal the fence, so we can carry on with the code.

    //Get Next Image index and signal semaphore when reading to be drawn to:
    uint32_t imageIndex;
    vkAcquireNextImageKHR(vulkanBE.mainDevices.device, vulkanBE.swapChain, INT_MAX, vulkanBE.sync.imageAvailable[currentFrame], VK_NULL_HANDLE, &imageIndex);

    recordCommand(currentFrame);
    updateUniformBuffers(currentFrame);

    //submit command buffer to render
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &vulkanBE.sync.imageAvailable[currentFrame];
    VkPipelineStageFlags waitStages[] = { //as soon as this stage of the pipeline is reached, the semaphore will be waited for.
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1; //number of command buffers to submit
    submitInfo.pCommandBuffers = &vulkanBE.commandBuffers[imageIndex]; //command buffer to submit
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &vulkanBE.sync.renderFinished[currentFrame]; //semaphore to signal when command buffer is finished submitting

    //submit command buffer to queue
    VkResult result = vkQueueSubmit(vulkanBE.graphicsQueue, 1, &submitInfo, vulkanBE.sync.drawFences[currentFrame]);
    if(result != VK_SUCCESS)
    {
        fprintf(stderr, "failed to submit the command buffer to the graphics queue\n");
        exit(1);
    }

    // present rendered image to the screen
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &vulkanBE.sync.renderFinished[currentFrame]; //semaphore to wait on
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &vulkanBE.swapChain;
    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(vulkanBE.presentQueue, &presentInfo);
    if(result != VK_SUCCESS)
    {
        fprintf(stderr, "failed to present image\n");
        exit(1);
    }
    currentFrame = (currentFrame + 1) % MAX_FRAME_DRAWS;
}

static void run(){

    while(!glfwWindowShouldClose(ve_window.window)){
        glfwPollEvents();

        //imgui new frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();
        imGuiParametersSetup();
        ImGui::Render();

        gui.draw_data = ImGui::GetDrawData();

        updateModel();
        //updateCameraModel(0, model);

        draw();
    }

    glfwDestroyWindow(ve_window.window);
    cleanUpRenderer();
    cleanUpBackend(&vulkanBE);

    glfwTerminate();
}

static void initEngine(){
    renderer.run = run;
    initBackend();
    initGUI();
    loadModels();
    loadCameras();
    loadLights();
    //initMesh(&object1);
    for(int i = 0; i < object_list.size(); i++)
    {
        createVertexBuffer(&object_list[i]);
        createIndexBuffer(&object_list[i]);
    }
    createUniformBuffers();
}