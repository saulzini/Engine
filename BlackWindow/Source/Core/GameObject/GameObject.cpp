#include "GameObject.h"
#include "Core/Components/ComponentFactory.h"
#include "Core/GameObject/GameObjectTypes.h"
#include "GL/glew.h"
#include "Math/Quat.h"
#include <queue>          // std::queue
#include <stack>          // std::stack

#include "Application.h"
#include "ModuleEditor.h"
#include "UIWindow/ConsoleWindow.h"
#include "json/json.h"

Component* GameObject::AddComponent(ComponentTypes type)
{
    Component *component = ComponentFactory::CreateComponent( this,type);
    components.push_back( component );

    if (type == ComponentTypes::TRANSFORM){
        transformComponent = (ComponentTransform*) component;
    }

    return component;
}

void GameObject::AddChildren(GameObject* gameObject) 
{
    children.push_back(gameObject);
    gameObject->parent = this;
}

void GameObject::SetProjectionMatrix(const float4x4& projection) 
{
    projectionMatrix = projection;
}

void GameObject::SetViewMatrix(const float4x4& view) 
{
    viewMatrix = view;
}

void GameObject::CalculateModelMatrix() 
{
    modelMatrix = float4x4::identity;
    if (transformComponent){
        modelMatrix = float4x4::FromTRS(
            transformComponent->GetPosition(), 
            transformComponent->GetRotationQuat() , 
            transformComponent->GetScale()
        );
    }
}

bool GameObject::isChild(GameObject *lookingChild) 
{
        if (children.size()<= 0){
            return false;
        }

        std::queue<GameObject *> searchQueue;
        searchQueue.push(this);
        GameObject *current;
        while( !searchQueue.empty() ){
            current = searchQueue.front();
            searchQueue.pop();
            if (current == lookingChild){
                return true;
            }

            std::vector<GameObject *> currentChildren = current->GetChildren();

            if (currentChildren.size()> 0){
                for (std::vector<GameObject *>::iterator it = currentChildren.begin(); it != currentChildren.end(); ++it)
                {
                    searchQueue.push( (GameObject *)*it );
                }
            }
        }

        return false;
       
}







void GameObject::Save() 
{
    // if (children.size() <= 0){
        // return;
    // }
    Json::Value val;
    val["jojo"] = "aa";

    std::stack<GameObject *> searchStack;
    searchStack.push(this);
    GameObject *current = nullptr;
    // do {
    //     current = searchStack.top();

    //     std::vector<GameObject *> currentChildren = current->GetChildren();

    //     if (currentChildren.size()> 0){
    //         for (std::vector<GameObject *>::iterator it = currentChildren.begin(); it != currentChildren.end(); ++it)
    //         {
    //             searchStack.push( (GameObject *)*it );
    //         }
    //     }

    //     searchStack.pop();
    //     App->editor->consoleWindow->AddLog( "%s", current->GetName().c_str() );

        
     
    // } while( !searchStack.empty() );

}

void GameObject::Export() 
{
    
}


void GameObject::Update() 
{
     //Draw();
    
    for (std::vector<Component *>::iterator it = components.begin() ; it != components.end(); ++it){
        
		if ( ( (Component *) *it)->GetType() == ComponentTypes::MESH){
		    ( (Component *) *it)->Update();
        }
	}

    for (std::vector<GameObject *>::iterator it = children.begin() ; it != children.end(); ++it){
        ( (GameObject *) *it )->SetProjectionMatrix(projectionMatrix);
        ( (GameObject *) *it )->SetViewMatrix(viewMatrix);
        Draw();

        ( (GameObject *) *it)->Update();
	}

}


void GameObject::Draw() 
{
    // float4x4 identityModel = float4x4::identity;
    float4x4 calculatedModel = float4x4::identity;

    if (transformComponent){
        // calculatedModel = float4x4::FromTRS(transformComponent->GetTransform(), rotationQuat , transformComponent->GetScale());
        //Quat rotationQuat(0.0f, 0.0f, 0.0f, 0.0f);
        //float3 scaleVector(2.0f, 2.0f, 2.0f);
        //Quat rotationQuat(transformComponent->GetRotationQuat(),0.0f);
        calculatedModel = float4x4::FromTRS(transformComponent->GetPosition(), transformComponent->GetRotationQuat() , transformComponent->GetScale());
    }

    // modelMatrix = identityModel * calculatedModel;
    modelMatrix = calculatedModel;
    // CalculateModelMatrix();
    if (parent){
        modelMatrix = parent->modelMatrix * modelMatrix;
    }

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &modelMatrix[0][0]); //GL_TRUE transpose the matrix
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &projectionMatrix[0][0]);
}


