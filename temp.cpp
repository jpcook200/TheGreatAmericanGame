//auto& transform = m_player->getComponent<CTransform>();
////Capture input
//Vec2 inputDir(0, 0);
//auto& input = m_player->getComponent<CInput>();
//
//if (input.left)
//inputDir.x -= 1;
//if (input.right)
//inputDir.x += 1;
//if (input.up)    inputDir.y -= 1;
//if (input.down)  inputDir.y += 1;
//
//if (inputDir == Vec2(0, 0))
//{
//
//	bobTime += bobSpeed;   if you have deltaTime, multiply it here
//	transform.pos.y += std::sin(bobTime) * bobAmplitude;
//}
//
//
////Normalize input direction
//if (inputDir.length() > 0)
//inputDir = inputDir.normalized();
//
////Apply acceleration
//transform.velocity += inputDir * acceleration;
//
////Apply damping when no input
//if (inputDir.length() == 0)
//{
//	if (transform.velocity != Vec2(0, 0))
//		transform.velocity *= damping;
//
//	if (abs(transform.velocity.x) < 0.01f)
//		transform.velocity.x = 0;
//
//	else if (abs(transform.velocity.y) < 0.01f)
//		transform.velocity.y = 0;
//
//}
//
////Clamp max speed
//if (transform.velocity.length() > maxSpeed)
//transform.velocity = transform.velocity.normalized() * maxSpeed;
//
//
////point player to mouse
//sf::Vector2f mousePos = m_game->getWindow().mapPixelToCoords(sf::Mouse::getPosition(m_game->getWindow()));
//sf::Vector2f playerPos(transform.pos.x, transform.pos.y);
//sf::Vector2f direction = mousePos - playerPos;
//float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159f;
//
//if (direction.x < -flipTrigger)
//{
//	//Smooth flip
//	transform.scale.x += (-1.f - transform.scale.x) * flipSpeed;
//
//	//Invert so the back faces the mouse
//	angle += 180.f;   flip rotation for the back
//
//		//Keep angle in [-180,180] range
//		if (angle > 180.f) angle -= 360.f;
//
//	//Clamp for left-facing
//	if (angle < -angleClamp) angle = -angleClamp;
//	else if (angle > angleClamp) angle = angleClamp;
//
//	transform.angle = angle;
//}
//else if (direction.x > flipTrigger)
//{
//	//Smooth flip
//	transform.scale.x += (1.f - transform.scale.x) * flipSpeed;
//
//	//Clamp for right-facing
//	if (angle < -angleClamp) angle = -angleClamp;
//	else if (angle > angleClamp) angle = angleClamp;
//
//	transform.angle = angle;
//}
//
////Apply movement to all entities with a transform component
//for (auto& e : m_entityManager.getEntities())
//{
//	if (e->hasComponent<CTransform>())
//	{
//		auto& t = e->getComponent<CTransform>();
//		if (t.parent == nullptr)
//		{
//
//			t.prevPos = t.pos;
//			t.pos += t.velocity;
//		}
//		else
//		{
//			auto& p = t.parent->getComponent<CTransform>();
//
//			//Apply parent's scale to local offset (flipping effect)
//			Vec2 offset = t.offSet;
//			offset.x *= p.scale.x;  flip horizontally if parent flipped
//				offset.y *= p.scale.y;  flip vertically if needed
//
//				//Apply parent rotation if needed
//				float rad = p.angle * 3.14159f / 180.f;
//			Vec2 rotatedOffset = {
//				offset.x * cos(rad) - offset.y * sin(rad),
//				offset.x * sin(rad) + offset.y * cos(rad)
//			};
//
//			//World position = parent pos + transformed offset
//			t.prevPos = t.pos;
//			t.pos = p.pos + rotatedOffset;
//
//			// Inherit scale from parent
//			if (e->getTag() != "player_gun")
//				t.scale = p.scale;
//		}
//	}
//	if (e->getTag() == "player_gun")
//	{
//		auto& t = e->getComponent<CTransform>();
//		auto& parentT = t.parent->getComponent<CTransform>();
//
//		// Get global position of gun relative to parent
//		Vec2 gunPos = parentT.pos + Vec2(t.offSet.x * parentT.scale.x, t.offSet.y);
//
//		//Calculate angle to mouse
//		sf::Vector2f mousePos = m_game->getWindow().mapPixelToCoords(sf::Mouse::getPosition(m_game->getWindow()));
//		sf::Vector2f direction = mousePos - sf::Vector2f(gunPos.x, gunPos.y);
//
//		float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159f;
//		//Apply to gun transform
//		t.angle = angle;
//		t.scale.y = parentT.scale.x < 0 ? -1.f : 1.f;
//		//Update sprite
//		Animation& gunAnim = e->getComponent<CAnimation>().animation;
//		gunAnim.getSprite().setOrigin({ 0.5, gunAnim.getSize().y / 2 });
//
//	}
//}